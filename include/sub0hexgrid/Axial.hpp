#pragma once

#include <compare>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace sub0hexgrid {
/// Integer lattice value. All q/r values are valid; implicit s requires int64 arithmetic.
struct Axial {
    std::int32_t q{};
    std::int32_t r{};
    auto operator<=>(const Axial&) const = default;
};
static_assert(std::is_trivially_copyable_v<Axial>);

/// Six steps in positive-x, positive-y pointy-top world coordinates.
enum class Direction : std::uint8_t {
    east, northeast, northwest, west, southwest, southeast
};

/// Returns the adjacent cell, or nullopt for invalid direction or q/r overflow.
/// Stateless and allocation-free; no map membership/clipping is implied.
[[nodiscard]] std::optional<Axial> TryNeighbor(Axial cell, Direction direction) noexcept;

/// Exact lattice steps, using wide intermediates across the complete int32 q/r domain.
[[nodiscard]] std::uint64_t ComputeDistance(Axial from, Axial to) noexcept;
}
