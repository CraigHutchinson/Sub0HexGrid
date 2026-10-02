#pragma once

#include <optional>

#include <sub0hexgrid/Axial.hpp>
#include <sub0hexgrid/Point.hpp>

namespace sub0hexgrid {
/// Validated regular pointy-top geometry. Const operations are stateless/allocation-free.
/// See docs/decisions/0001-coordinate-layout.md for axes, scale and rounding conventions.
class PointyLayout {
public:
    /// Positive finite circumradius and finite origin; invalid geometry returns nullopt.
    [[nodiscard]] static std::optional<PointyLayout> TryCreate(
        double radius, Point origin = {}) noexcept;

    /// Returns a finite center that maps back to the cell; overflow or precision collapse returns nullopt.
    [[nodiscard]] std::optional<Point> TryCellCenter(Axial cell) const noexcept;

    /// Returns the rounded cell, or nullopt for nonfinite/unrepresentable conversions.
    /// Cube rounding repairs the largest residual; exact computed ties prefer q, r, s.
    /// Rounding is half away from zero. Near-seam cross-platform identity is not promised.
    [[nodiscard]] std::optional<Axial> TryCellAt(Point position) const noexcept;

    /// Circumradius in world units, shared by conservative candidate bounds.
    [[nodiscard]] double GetRadius() const noexcept { return m_Radius; }

    /// World-space translation used by candidate normalization.
    [[nodiscard]] Point GetOrigin() const noexcept { return m_Origin; }

private:
    PointyLayout(double radius, Point origin) noexcept;
    double m_Radius;
    Point m_Origin;
};
}
