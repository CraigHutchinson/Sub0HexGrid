#pragma once

#include <cstdint>
#include <optional>
#include <type_traits>

#include "sub0hexgrid/axial.hpp"

namespace sub0hexgrid
{
/** Validated finite axial rectangle addressing application-owned contiguous storage.
 * Owns metadata only; concurrent const operations are safe.
 */
class AxialRegion
{
  public:
    /** Creates a nonempty region with inclusive bounds.
     * @param minimum Lowest admitted q/r coordinates.
     * @param maximum Highest admitted q/r coordinates.
     * @return nullopt for reversed bounds or a cell count exceeding uint64.
     */
    [[nodiscard]] static std::optional<AxialRegion> tryCreate(Axial minimum,
                                                              Axial maximum) noexcept;

    /** Returns the inclusive minimum coordinate.
     * @return The lowest admitted q/r coordinate.
     */
    [[nodiscard]] constexpr Axial getMinimum() const noexcept { return minimum_; }

    /** Returns the inclusive maximum coordinate.
     * @return The highest admitted q/r coordinate.
     */
    [[nodiscard]] constexpr Axial getMaximum() const noexcept { return maximum_; }

    /** Returns the number of addressed cells.
     * @return The region's cell count.
     */
    [[nodiscard]] constexpr std::uint64_t getCellCount() const noexcept { return cellCount_; }

    /** Reports whether a coordinate lies within both inclusive bounds.
     * @param cell Coordinate to check.
     * @return True when the region contains the coordinate.
     */
    [[nodiscard]] constexpr bool contains(Axial cell) const noexcept
    {
        return cell.q >= minimum_.q && cell.q <= maximum_.q && cell.r >= minimum_.r &&
               cell.r <= maximum_.r;
    }

    /** Maps a cell to its zero-based row-major index, with r outer and q inner.
     * @param cell Coordinate to address.
     * @return The storage index, or nullopt when the cell is outside this region.
     */
    [[nodiscard]] std::optional<std::uint64_t> tryIndex(Axial cell) const noexcept;

    /** Maps a zero-based row-major index back to its coordinate.
     * @param index Storage position; it must be less than getCellCount().
     * @return The coordinate, or nullopt when index is outside this region.
     */
    [[nodiscard]] std::optional<Axial> tryCell(std::uint64_t index) const noexcept;

  private:
    constexpr AxialRegion(Axial minimum, Axial maximum, std::uint64_t width,
                          std::uint64_t cellCount) noexcept
        : minimum_(minimum), maximum_(maximum), width_(width), cellCount_(cellCount)
    {
    }

    Axial minimum_;
    Axial maximum_;
    std::uint64_t width_;
    std::uint64_t cellCount_;
};
static_assert(std::is_trivially_copyable_v<AxialRegion>);
} // namespace sub0hexgrid
