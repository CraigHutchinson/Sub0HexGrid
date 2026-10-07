#include "sub0hexgrid/regions/axial_region.hpp"

#include <limits>

namespace sub0hexgrid
{
std::optional<AxialRegion> AxialRegion::tryCreate(Axial minimum, Axial maximum) noexcept
{
    if (minimum.q > maximum.q || minimum.r > maximum.r)
        return std::nullopt;
    const auto width =
        static_cast<std::uint64_t>(static_cast<std::int64_t>(maximum.q) - minimum.q + 1);
    const auto height =
        static_cast<std::uint64_t>(static_cast<std::int64_t>(maximum.r) - minimum.r + 1);
    if (width > std::numeric_limits<std::uint64_t>::max() / height)
        return std::nullopt;
    return AxialRegion{minimum, maximum, width, width * height};
}

std::optional<std::uint64_t> AxialRegion::tryIndex(Axial cell) const noexcept
{
    if (!contains(cell))
        return std::nullopt;
    const auto column = static_cast<std::uint64_t>(static_cast<std::int64_t>(cell.q) - minimum_.q);
    const auto row = static_cast<std::uint64_t>(static_cast<std::int64_t>(cell.r) - minimum_.r);
    return row * width_ + column;
}

std::optional<Axial> AxialRegion::tryCell(std::uint64_t index) const noexcept
{
    if (index >= cellCount_)
        return std::nullopt;
    const auto q =
        static_cast<std::int64_t>(minimum_.q) + static_cast<std::int64_t>(index % width_);
    const auto r =
        static_cast<std::int64_t>(minimum_.r) + static_cast<std::int64_t>(index / width_);
    return Axial{static_cast<std::int32_t>(q), static_cast<std::int32_t>(r)};
}
} // namespace sub0hexgrid
