#include "sub0hexgrid/axial.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <limits>

namespace sub0hexgrid
{
std::optional<Axial> tryNeighbor(Axial cell, Direction direction) noexcept
{
    constexpr std::array<Axial, 6> steps{{{1, 0}, {1, -1}, {0, -1}, {-1, 0}, {-1, 1}, {0, 1}}};
    const auto index = static_cast<std::uint8_t>(direction);
    if (index >= steps.size())
        return std::nullopt;
    const auto q = static_cast<std::int64_t>(cell.q) + steps[index].q;
    const auto r = static_cast<std::int64_t>(cell.r) + steps[index].r;
    constexpr auto minimum = std::numeric_limits<std::int32_t>::min();
    constexpr auto maximum = std::numeric_limits<std::int32_t>::max();
    if (q < minimum || q > maximum || r < minimum || r > maximum)
        return std::nullopt;
    return Axial{static_cast<std::int32_t>(q), static_cast<std::int32_t>(r)};
}

std::uint64_t computeDistance(Axial from, Axial to) noexcept
{
    const auto dq = static_cast<std::int64_t>(from.q) - to.q;
    const auto dr = static_cast<std::int64_t>(from.r) - to.r;
    return static_cast<std::uint64_t>(std::max({std::abs(dq), std::abs(dr), std::abs(dq + dr)}));
}
} // namespace sub0hexgrid
