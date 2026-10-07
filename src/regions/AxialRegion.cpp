#include "sub0hexgrid/regions/AxialRegion.hpp"

#include <limits>

namespace sub0hexgrid {
std::optional<AxialRegion> AxialRegion::TryCreate(Axial minimum, Axial maximum) noexcept {
    if (minimum.q > maximum.q || minimum.r > maximum.r) return std::nullopt;
    const auto width = static_cast<std::uint64_t>(static_cast<std::int64_t>(maximum.q) - minimum.q + 1);
    const auto height = static_cast<std::uint64_t>(static_cast<std::int64_t>(maximum.r) - minimum.r + 1);
    if (width > std::numeric_limits<std::uint64_t>::max() / height) return std::nullopt;
    return AxialRegion{minimum, maximum, width, width * height};
}

std::optional<std::uint64_t> AxialRegion::TryIndex(Axial cell) const noexcept {
    if (!Contains(cell)) return std::nullopt;
    const auto column = static_cast<std::uint64_t>(static_cast<std::int64_t>(cell.q) - m_Minimum.q);
    const auto row = static_cast<std::uint64_t>(static_cast<std::int64_t>(cell.r) - m_Minimum.r);
    return row * m_Width + column;
}

std::optional<Axial> AxialRegion::TryCell(std::uint64_t index) const noexcept {
    if (index >= m_CellCount) return std::nullopt;
    const auto q = static_cast<std::int64_t>(m_Minimum.q) + static_cast<std::int64_t>(index % m_Width);
    const auto r = static_cast<std::int64_t>(m_Minimum.r) + static_cast<std::int64_t>(index / m_Width);
    return Axial{static_cast<std::int32_t>(q), static_cast<std::int32_t>(r)};
}
}
