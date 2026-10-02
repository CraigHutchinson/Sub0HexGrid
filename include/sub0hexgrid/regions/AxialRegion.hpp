#pragma once

#include <cstdint>
#include <optional>
#include <type_traits>

#include <sub0hexgrid/Axial.hpp>

namespace sub0hexgrid {
/** Validated finite axial rectangle addressing application-owned contiguous storage.
 * Owns metadata only; concurrent const operations are safe.
 */
class AxialRegion {
public:
    /** Creates a nonempty region with inclusive bounds.
     * @param minimum Lowest admitted q/r coordinates.
     * @param maximum Highest admitted q/r coordinates.
     * @return nullopt for reversed bounds or a cell count exceeding uint64.
     */
    [[nodiscard]] static std::optional<AxialRegion> TryCreate(Axial minimum, Axial maximum) noexcept;

    [[nodiscard]] constexpr Axial GetMinimum() const noexcept { return m_Minimum; }
    [[nodiscard]] constexpr Axial GetMaximum() const noexcept { return m_Maximum; }
    [[nodiscard]] constexpr std::uint64_t GetCellCount() const noexcept { return m_CellCount; }

    /// Returns whether cell lies within both inclusive coordinate bounds.
    [[nodiscard]] constexpr bool Contains(Axial cell) const noexcept {
        return cell.q >= m_Minimum.q && cell.q <= m_Maximum.q &&
               cell.r >= m_Minimum.r && cell.r <= m_Maximum.r;
    }

    /** Maps a cell to its zero-based row-major index, with r outer and q inner.
     * @param cell Coordinate to address.
     * @return nullopt when cell is outside this region.
     */
    [[nodiscard]] std::optional<std::uint64_t> TryIndex(Axial cell) const noexcept;

    /** Maps a zero-based row-major index back to its coordinate.
     * @param index Storage position; must be less than GetCellCount().
     * @return nullopt when index is outside this region.
     */
    [[nodiscard]] std::optional<Axial> TryCell(std::uint64_t index) const noexcept;

private:
    constexpr AxialRegion(Axial minimum, Axial maximum, std::uint64_t width,
                          std::uint64_t cellCount) noexcept
        : m_Minimum(minimum), m_Maximum(maximum), m_Width(width), m_CellCount(cellCount) {}

    Axial m_Minimum;
    Axial m_Maximum;
    std::uint64_t m_Width;
    std::uint64_t m_CellCount;
};
static_assert(std::is_trivially_copyable_v<AxialRegion>);
}
