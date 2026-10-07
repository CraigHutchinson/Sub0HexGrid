#pragma once

#include <compare>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace sub0hexgrid
{
/// Integer lattice value. All q/r values are valid; implicit s requires int64 arithmetic.
struct Axial
{
    std::int32_t q{};
    std::int32_t r{};
    auto operator<=>(const Axial&) const = default;
};
static_assert(std::is_trivially_copyable_v<Axial>);

/// Six steps in positive-x, positive-y pointy-top world coordinates.
enum class Direction : std::uint8_t
{
    east,
    northeast,
    northwest,
    west,
    southwest,
    southeast
};

/** Returns the adjacent cell when the direction and coordinate arithmetic are valid.
 * The operation is stateless and allocation-free; it does not check map membership.
 * @param cell Source lattice coordinate.
 * @param direction One of the six lattice directions.
 * @return The adjacent coordinate, or nullopt for an invalid direction or q/r overflow.
 */
[[nodiscard]] std::optional<Axial> tryNeighbor(Axial cell, Direction direction) noexcept;

/** Returns the exact lattice distance using wide intermediates over the int32 q/r domain.
 * @param from First lattice coordinate.
 * @param to Second lattice coordinate.
 * @return The number of lattice steps between the coordinates.
 */
[[nodiscard]] std::uint64_t computeDistance(Axial from, Axial to) noexcept;
} // namespace sub0hexgrid
