#pragma once

#include <optional>

#include "sub0hexgrid/axial.hpp"
#include "sub0hexgrid/point.hpp"

namespace sub0hexgrid
{
/// Validated regular pointy-top geometry. Const operations are stateless/allocation-free.
/// See docs/decisions/0001-coordinate-layout.md for axes, scale and rounding conventions.
class PointyLayout
{
  public:
    /** Creates validated pointy-top geometry.
     * @param radius Positive finite circumradius in world units.
     * @param origin Finite world-space location of the origin cell.
     * @return The layout, or nullopt when either input is invalid.
     */
    [[nodiscard]] static std::optional<PointyLayout> tryCreate(double radius,
                                                               Point origin = {}) noexcept;

    /** Returns the world-space center of a cell.
     * @param cell Lattice coordinate to convert.
     * @return A finite center that maps back to the cell, or nullopt on overflow or precision
     * collapse.
     */
    [[nodiscard]] std::optional<Point> tryCellCenter(Axial cell) const noexcept;

    /** Maps a world-space point to its nearest lattice cell.
     * Cube rounding repairs the largest residual; exact computed ties prefer q, r, s.
     * Rounding is half away from zero. Near-seam cross-platform identity is not promised.
     * @param position Finite world-space position to map.
     * @return The rounded cell, or nullopt for nonfinite or unrepresentable conversions.
     */
    [[nodiscard]] std::optional<Axial> tryCellAt(Point position) const noexcept;

    /** Returns the circumradius in world units.
     * @return The validated circumradius used by conservative candidate bounds.
     */
    [[nodiscard]] double getRadius() const noexcept { return radius_; }

    /** Returns the world-space origin translation.
     * @return The translation used by coordinate normalization.
     */
    [[nodiscard]] Point getOrigin() const noexcept { return origin_; }

  private:
    PointyLayout(double radius, Point origin) noexcept;
    double radius_;
    Point origin_;
};
} // namespace sub0hexgrid
