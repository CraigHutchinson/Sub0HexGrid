#pragma once

#include <cstdint>
#include <iterator>
#include <optional>

#include "sub0hexgrid/Axial.hpp"
#include "sub0hexgrid/Point.hpp"
#include "sub0hexgrid/PointyLayout.hpp"
#include "sub0hexgrid/regions/AxialRegion.hpp"

namespace sub0hexgrid {
/** Allocation-free conservative candidates for an inclusive world-space disk.
 * Owns scalar metadata, with no layout, region or caller-buffer borrows.
 * Const ranges and independently copied iterators can be read concurrently.
 */
class CandidateCells {
public:
    /** Forward iterator yielding Axial values in ascending r, then q order.
     * Iterator metadata is owned, so destroying its source range does not invalidate it.
     */
    class Iterator {
    public:
        using value_type = Axial;
        using difference_type = std::int64_t;
        using reference = Axial;
        using iterator_concept = std::forward_iterator_tag;
        using iterator_category = std::input_iterator_tag;

        /// Creates a singular iterator, comparable to another default iterator.
        Iterator() noexcept = default;

        /// Returns the current cell by value; dereferencing end is invalid.
        [[nodiscard]] Axial operator*() const noexcept;
        /// Advances one cell; incrementing end is invalid.
        Iterator& operator++() noexcept;
        /// Advances one cell and returns the previous position.
        Iterator operator++(int) noexcept;
        [[nodiscard]] bool operator==(const Iterator&) const noexcept = default;

    private:
        friend class CandidateCells;
        Iterator(Axial minimum, std::uint64_t width, std::uint64_t index) noexcept;
        Axial m_Minimum{};
        std::uint64_t m_Width{1};
        std::uint64_t m_Index{};
    };

    /** Constructs clipped candidates in constant work, including a valid empty intersection.
     * Includes every region cell mapped by a point inside the disk; false positives remain.
     * Requires IEEE binary64 round-to-nearest arithmetic without fast-math.
     * @param layout Validated pointy geometry used to map entity positions.
     * @param region Finite region containing the application's resident cells.
     * @param center Finite world-space center of the inclusive disk.
     * @param radius Nonnegative finite world-space disk radius, including zero.
     * @return Candidates, or nullopt for invalid/nonfinite arithmetic or normalized
     *         q/r interval endpoints beyond magnitude 2^40, even if clipping could succeed.
     */
    [[nodiscard]] static std::optional<CandidateCells> TryCreate(
        PointyLayout layout, AxialRegion region, Point center, double radius) noexcept;

    /// Returns the exact number of candidates; use this for counts exceeding INT64_MAX.
    [[nodiscard]] constexpr std::uint64_t GetCellCount() const noexcept { return m_Count; }

    /** Partitions the current range in constant work, preserving its original order.
     * @param first Zero-based offset within this range; an empty end slice is valid.
     * @param count Number of cells in the slice.
     * @return An owned slice, or nullopt if first/count exceed the current range.
     */
    [[nodiscard]] std::optional<CandidateCells> TrySlice(
        std::uint64_t first, std::uint64_t count) const noexcept;

    /** Returns the first iterator; empty ranges return end.
     * Standard iterator distances require the count to fit difference_type.
     */
    [[nodiscard]] Iterator begin() const noexcept;
    /// Returns the one-past-last iterator.
    [[nodiscard]] Iterator end() const noexcept;

private:
    friend class CandidateCursor;
    CandidateCells(Axial minimum, std::uint64_t width,
        std::uint64_t first, std::uint64_t count) noexcept;
    [[nodiscard]] static Axial CellAt(
        Axial minimum, std::uint64_t width, std::uint64_t index) noexcept;
    [[nodiscard]] Axial CellAt(std::uint64_t offset) const noexcept;
    Axial m_Minimum;
    std::uint64_t m_Width;
    std::uint64_t m_First;
    std::uint64_t m_Count;
};
}
