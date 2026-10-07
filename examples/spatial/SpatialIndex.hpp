#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <stdexcept>
#include <vector>

#include "sub0hexgrid/candidates/candidate_cursor.hpp"

namespace sub0hexgrid::example
{
/// Application-owned bins demonstrating resumable builds and snapshot-bound queries.
class SpatialIndex
{
  public:
    /// Build status; failed/cancelled rebuilds preserve the previous committed snapshot.
    enum class BuildState
    {
        idle,
        working,
        complete,
        failed
    };

    /// A written prefix and bounded work count; invalid means the snapshot was replaced.
    struct QueryBatch
    {
        std::size_t written_{};
        std::size_t work_{};
        bool done_{};
        bool valid_{true};
    };

    /** Resumable query over a committed snapshot; copied queries have independent progress.
     * The index is non-owning and must outlive this cursor. Reads require no concurrent
     * index mutation. Rebuilding publication invalidates outstanding cursors explicitly.
     */
    class Query
    {
      public:
        /** Writes matching snapshot row indices into the output prefix.
         * Each cell visit or entity examination consumes one work unit; zero budget/output
         * yields without progress. Callers check deadlines between calls and retain all
         * prefixes until done; an invalidated query's partial output must be discarded.
         */
        [[nodiscard]] QueryBatch read(std::span<std::size_t> output, std::size_t budget) noexcept
        {
            if (version_ != index_->version_)
                return {0, 0, false, false};
            QueryBatch batch{};
            if (output.empty() || budget == 0) {
                batch.done_ = candidates_.isDone() && bin_ == end_;
                return batch;
            }
            while (batch.work_ < budget && batch.written_ < output.size()) {
                if (bin_ < end_) {
                    const auto row = index_->bins_[bin_++];
                    const auto point = index_->points_[row];
                    ++batch.work_;
                    if (std::hypot(point.x - center_.x, point.y - center_.y) <= radius_)
                        output[batch.written_++] = row;
                } else {
                    Axial cell{};
                    if (candidates_.read(std::span{&cell, 1}) == 0)
                        break;
                    const auto index = static_cast<std::size_t>(*index_->region_.tryIndex(cell));
                    bin_ = index_->offsets_[index];
                    end_ = index_->offsets_[index + 1];
                    ++batch.work_;
                }
            }
            batch.done_ = candidates_.isDone() && bin_ == end_;
            return batch;
        }

      private:
        friend class SpatialIndex;
        Query(const SpatialIndex& index, CandidateCells cells, Point center, double radius) noexcept
            : index_(&index), version_(index.version_), candidates_(cells), center_(center),
              radius_(radius)
        {
        }
        const SpatialIndex* index_; // non-owning; snapshot lifetime is checked by version
        std::uint64_t version_;
        CandidateCursor candidates_;
        Point center_;
        double radius_;
        std::size_t bin_{};
        std::size_t end_{};
    };

    /// Validates host capacity and preallocates all current/pending storage; may throw at startup.
    SpatialIndex(PointyLayout layout, AxialRegion region, std::size_t capacity)
        : layout_(layout), region_(region)
    {
        const auto count = region.getCellCount();
        if (count >= offsets_.max_size() || capacity > points_.max_size() ||
            capacity > bins_.max_size())
            throw std::length_error("Spatial index capacity");
        const auto cells = static_cast<std::size_t>(count);
        points_.resize(capacity);
        pendingPoints_.resize(capacity);
        sampleCells_.resize(capacity);
        bins_.resize(capacity);
        pendingBins_.resize(capacity);
        counts_.resize(cells);
        cursors_.resize(cells);
        offsets_.resize(cells + 1);
        pendingOffsets_.resize(cells + 1);
    }

    SpatialIndex(const SpatialIndex&) = delete;
    SpatialIndex& operator=(const SpatialIndex&) = delete;
    SpatialIndex(SpatialIndex&&) = delete;
    SpatialIndex& operator=(SpatialIndex&&) = delete;

    /** Begins an O(1) rebuild; the input borrow must remain immutable/alive until
     * completion/cancel. Rejects active builds, excess capacity or exhausted version without
     * changing any state. Invalid point/region membership is detected by stepRebuild before
     * publication.
     */
    [[nodiscard]] bool beginRebuild(std::span<const Point> input) noexcept
    {
        if (state_ == BuildState::working || input.size() > points_.size() ||
            version_ == std::numeric_limits<std::uint64_t>::max())
            return false;
        input_ = input;
        phase_ = Phase::reset;
        row_ = 0;
        state_ = BuildState::working;
        return true;
    }

    /** Performs at most budget cell/sample work units, then yields; zero makes no progress.
     * Reset, assignment, prefix scan and scatter are all resumable. Publication is one swap
     * after complete validation; all retained queries require an unchanged committed snapshot.
     */
    [[nodiscard]] BuildState stepRebuild(std::size_t budget) noexcept
    {
        std::size_t work = 0;
        while (state_ == BuildState::working && work < budget) {
            switch (phase_) {
            case Phase::reset:
                if (row_ == counts_.size()) {
                    phase_ = Phase::assign;
                    row_ = 0;
                    break;
                }
                counts_[row_++] = 0;
                ++work;
                break;
            case Phase::assign:
                if (row_ == input_.size()) {
                    phase_ = Phase::prefix;
                    row_ = 0;
                    pendingOffsets_[0] = 0;
                    break;
                }
                if (const auto cell = layout_.tryCellAt(input_[row_])) {
                    if (const auto index = region_.tryIndex(*cell)) {
                        const auto host = static_cast<std::size_t>(*index);
                        sampleCells_[row_] = host;
                        pendingPoints_[row_] = input_[row_];
                        ++counts_[host];
                        ++row_;
                        ++work;
                        break;
                    }
                }
                state_ = BuildState::failed;
                input_ = {};
                break;
            case Phase::prefix:
                if (row_ == counts_.size()) {
                    phase_ = Phase::scatter;
                    row_ = 0;
                    break;
                }
                pendingOffsets_[row_ + 1] = pendingOffsets_[row_] + counts_[row_];
                cursors_[row_] = pendingOffsets_[row_];
                ++row_;
                ++work;
                break;
            case Phase::scatter:
                if (row_ == input_.size()) {
                    points_.swap(pendingPoints_);
                    bins_.swap(pendingBins_);
                    offsets_.swap(pendingOffsets_);
                    sampleCount_ = input_.size();
                    input_ = {};
                    ++version_;
                    state_ = BuildState::complete;
                    break;
                }
                pendingBins_[cursors_[sampleCells_[row_]]++] = row_;
                ++row_;
                ++work;
                break;
            }
        }
        return state_;
    }

    /// Releases an input borrow and abandons pending work; committed bins remain unchanged.
    void cancelRebuild() noexcept
    {
        input_ = {};
        state_ = BuildState::idle;
    }

    /// Constructs a snapshot-bound query without retaining output storage or allocating.
    [[nodiscard]] std::optional<Query> tryQuery(Point center, double radius) const noexcept
    {
        const auto cells = CandidateCells::tryCreate(layout_, region_, center, radius);
        if (!cells)
            return std::nullopt;
        return Query{*this, *cells, center, radius};
    }

    /// Committed sample count; failed/pending work does not affect this observation.
    [[nodiscard]] std::size_t getSampleCount() const noexcept { return sampleCount_; }

    /// Resident vector-capacity payload, excluding vector objects, allocator overhead and input.
    [[nodiscard]] std::uint64_t getStorageBytes() const noexcept
    {
        return (static_cast<std::uint64_t>(points_.capacity()) + pendingPoints_.capacity()) *
                   sizeof(Point) +
               (static_cast<std::uint64_t>(sampleCells_.capacity()) + bins_.capacity() +
                pendingBins_.capacity() + counts_.capacity() + cursors_.capacity() +
                offsets_.capacity() + pendingOffsets_.capacity()) *
                   sizeof(std::size_t);
    }

  private:
    enum class Phase
    {
        reset,
        assign,
        prefix,
        scatter
    };
    PointyLayout layout_;
    AxialRegion region_;
    std::vector<Point> points_, pendingPoints_;
    std::vector<std::size_t> sampleCells_, bins_, pendingBins_;
    std::vector<std::size_t> counts_, cursors_, offsets_, pendingOffsets_;
    std::span<const Point> input_; // non-owning while working; released on cancel/fail/commit
    std::uint64_t version_{};
    std::size_t sampleCount_{}, row_{};
    Phase phase_{Phase::reset};
    BuildState state_{BuildState::idle};
};
} // namespace sub0hexgrid::example
