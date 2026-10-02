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

#include <sub0hexgrid/candidates/CandidateCursor.hpp>

namespace sub0hexgrid::example {
/// Application-owned bins demonstrating resumable builds and snapshot-bound queries.
class SpatialIndex {
public:
    /// Build status; failed/cancelled rebuilds preserve the previous committed snapshot.
    enum class BuildState { idle, working, complete, failed };

    /// A written prefix and bounded work count; invalid means the snapshot was replaced.
    struct QueryBatch {
        std::size_t written{};
        std::size_t work{};
        bool done{};
        bool valid{true};
    };

    /** Resumable query over a committed snapshot; copied queries have independent progress.
     * The index is non-owning and must outlive this cursor. Reads require no concurrent
     * index mutation. Rebuilding publication invalidates outstanding cursors explicitly.
     */
    class Query {
    public:
        /** Writes matching snapshot row indices into the output prefix.
         * Each cell visit or entity examination consumes one work unit; zero budget/output
         * yields without progress. Callers check deadlines between calls and retain all
         * prefixes until done; an invalidated query's partial output must be discarded.
         */
        [[nodiscard]] QueryBatch Read(std::span<std::size_t> output, std::size_t budget) noexcept {
            if (m_Version != m_Index->m_Version) return {0, 0, false, false};
            QueryBatch batch{};
            if (output.empty() || budget == 0) {
                batch.done = m_Candidates.IsDone() && m_Bin == m_End;
                return batch;
            }
            while (batch.work < budget && batch.written < output.size()) {
                if (m_Bin < m_End) {
                    const auto row = m_Index->m_Bins[m_Bin++];
                    const auto point = m_Index->m_Points[row];
                    ++batch.work;
                    if (std::hypot(point.x - m_Center.x, point.y - m_Center.y) <= m_Radius)
                        output[batch.written++] = row;
                } else {
                    Axial cell{};
                    if (m_Candidates.Read(std::span{&cell, 1}) == 0) break;
                    const auto index = static_cast<std::size_t>(*m_Index->m_Region.TryIndex(cell));
                    m_Bin = m_Index->m_Offsets[index];
                    m_End = m_Index->m_Offsets[index + 1];
                    ++batch.work;
                }
            }
            batch.done = m_Candidates.IsDone() && m_Bin == m_End;
            return batch;
        }

    private:
        friend class SpatialIndex;
        Query(const SpatialIndex& index, CandidateCells cells, Point center, double radius) noexcept
            : m_Index(&index), m_Version(index.m_Version), m_Candidates(cells),
              m_Center(center), m_Radius(radius) {}
        const SpatialIndex* m_Index; // non-owning; snapshot lifetime is checked by version
        std::uint64_t m_Version;
        CandidateCursor m_Candidates;
        Point m_Center;
        double m_Radius;
        std::size_t m_Bin{};
        std::size_t m_End{};
    };

    /// Validates host capacity and preallocates all current/pending storage; may throw at startup.
    SpatialIndex(PointyLayout layout, AxialRegion region, std::size_t capacity)
        : m_Layout(layout), m_Region(region) {
        const auto count = region.GetCellCount();
        if (count >= m_Offsets.max_size() || capacity > m_Points.max_size() ||
            capacity > m_Bins.max_size()) throw std::length_error("Spatial index capacity");
        const auto cells = static_cast<std::size_t>(count);
        m_Points.resize(capacity);
        m_PendingPoints.resize(capacity);
        m_SampleCells.resize(capacity);
        m_Bins.resize(capacity);
        m_PendingBins.resize(capacity);
        m_Counts.resize(cells);
        m_Cursors.resize(cells);
        m_Offsets.resize(cells + 1);
        m_PendingOffsets.resize(cells + 1);
    }

    SpatialIndex(const SpatialIndex&) = delete;
    SpatialIndex& operator=(const SpatialIndex&) = delete;
    SpatialIndex(SpatialIndex&&) = delete;
    SpatialIndex& operator=(SpatialIndex&&) = delete;

    /** Begins an O(1) rebuild; the input borrow must remain immutable/alive until completion/cancel.
     * Rejects active builds, excess capacity or exhausted version without changing any state.
     * Invalid point/region membership is detected by StepRebuild before publication.
     */
    [[nodiscard]] bool BeginRebuild(std::span<const Point> input) noexcept {
        if (m_State == BuildState::working || input.size() > m_Points.size() ||
            m_Version == std::numeric_limits<std::uint64_t>::max()) return false;
        m_Input = input;
        m_Phase = Phase::reset;
        m_Row = 0;
        m_State = BuildState::working;
        return true;
    }

    /** Performs at most budget cell/sample work units, then yields; zero makes no progress.
     * Reset, assignment, prefix scan and scatter are all resumable. Publication is one swap
     * after complete validation; all retained queries require an unchanged committed snapshot.
     */
    [[nodiscard]] BuildState StepRebuild(std::size_t budget) noexcept {
        std::size_t work = 0;
        while (m_State == BuildState::working && work < budget) {
            switch (m_Phase) {
            case Phase::reset:
                if (m_Row == m_Counts.size()) { m_Phase = Phase::assign; m_Row = 0; break; }
                m_Counts[m_Row++] = 0;
                ++work;
                break;
            case Phase::assign:
                if (m_Row == m_Input.size()) {
                    m_Phase = Phase::prefix; m_Row = 0; m_PendingOffsets[0] = 0; break;
                }
                if (const auto cell = m_Layout.TryCellAt(m_Input[m_Row])) {
                    if (const auto index = m_Region.TryIndex(*cell)) {
                        const auto host = static_cast<std::size_t>(*index);
                        m_SampleCells[m_Row] = host;
                        m_PendingPoints[m_Row] = m_Input[m_Row];
                        ++m_Counts[host];
                        ++m_Row;
                        ++work;
                        break;
                    }
                }
                m_State = BuildState::failed;
                m_Input = {};
                break;
            case Phase::prefix:
                if (m_Row == m_Counts.size()) { m_Phase = Phase::scatter; m_Row = 0; break; }
                m_PendingOffsets[m_Row + 1] = m_PendingOffsets[m_Row] + m_Counts[m_Row];
                m_Cursors[m_Row] = m_PendingOffsets[m_Row];
                ++m_Row;
                ++work;
                break;
            case Phase::scatter:
                if (m_Row == m_Input.size()) {
                    m_Points.swap(m_PendingPoints);
                    m_Bins.swap(m_PendingBins);
                    m_Offsets.swap(m_PendingOffsets);
                    m_SampleCount = m_Input.size();
                    m_Input = {};
                    ++m_Version;
                    m_State = BuildState::complete;
                    break;
                }
                m_PendingBins[m_Cursors[m_SampleCells[m_Row]]++] = m_Row;
                ++m_Row;
                ++work;
                break;
            }
        }
        return m_State;
    }

    /// Releases an input borrow and abandons pending work; committed bins remain unchanged.
    void CancelRebuild() noexcept { m_Input = {}; m_State = BuildState::idle; }

    /// Constructs a snapshot-bound query without retaining output storage or allocating.
    [[nodiscard]] std::optional<Query> TryQuery(Point center, double radius) const noexcept {
        const auto cells = CandidateCells::TryCreate(m_Layout, m_Region, center, radius);
        if (!cells) return std::nullopt;
        return Query{*this, *cells, center, radius};
    }

    /// Committed sample count; failed/pending work does not affect this observation.
    [[nodiscard]] std::size_t GetSampleCount() const noexcept { return m_SampleCount; }

    /// Resident vector-capacity payload, excluding vector objects, allocator overhead and input.
    [[nodiscard]] std::uint64_t GetStorageBytes() const noexcept {
        return (static_cast<std::uint64_t>(m_Points.capacity()) + m_PendingPoints.capacity()) * sizeof(Point) +
               (static_cast<std::uint64_t>(m_SampleCells.capacity()) + m_Bins.capacity() +
                m_PendingBins.capacity() + m_Counts.capacity() + m_Cursors.capacity() +
                m_Offsets.capacity() + m_PendingOffsets.capacity()) * sizeof(std::size_t);
    }

private:
    enum class Phase { reset, assign, prefix, scatter };
    PointyLayout m_Layout;
    AxialRegion m_Region;
    std::vector<Point> m_Points, m_PendingPoints;
    std::vector<std::size_t> m_SampleCells, m_Bins, m_PendingBins;
    std::vector<std::size_t> m_Counts, m_Cursors, m_Offsets, m_PendingOffsets;
    std::span<const Point> m_Input; // non-owning while working; released on cancel/fail/commit
    std::uint64_t m_Version{};
    std::size_t m_SampleCount{}, m_Row{};
    Phase m_Phase{Phase::reset};
    BuildState m_State{BuildState::idle};
};
}
