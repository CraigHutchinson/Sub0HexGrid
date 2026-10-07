#include <doctest/doctest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <ostream>
#include <map>
#include <numbers>
#include <queue>
#include <string_view>

#include "sub0hexgrid/pointy_layout.hpp"

namespace
{
void check(bool condition, std::string_view message) { REQUIRE_MESSAGE(condition, message); }

void checkGeometry()
{
    using namespace sub0hexgrid;
    constexpr auto maximum = std::numeric_limits<double>::max();
    constexpr auto infinity = std::numeric_limits<double>::infinity();
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    for (const double bad : std::array{0.0, -1.0, infinity, nan})
        check(!PointyLayout::tryCreate(bad), "invalid radius");
    check(!PointyLayout::tryCreate(1, {nan, 0}), "invalid origin x");
    check(!PointyLayout::tryCreate(1, {0, infinity}), "invalid origin y");

    const auto layout = PointyLayout::tryCreate(2, {10, 20});
    check(layout.has_value(), "valid layout");
    const auto east = layout->tryCellCenter({1, 0});
    const auto southeast = layout->tryCellCenter({0, 1});
    check(east && std::abs(east->x - (10 + std::sqrt(12.0))) < 1e-12 && east->y == 20,
          "analytic east center");
    check(southeast && std::abs(southeast->x - (10 + std::sqrt(3.0))) < 1e-12 && southeast->y == 23,
          "analytic southeast center");
    for (int q = -30; q <= 30; ++q) {
        for (int r = -30; r <= 30; ++r) {
            const Axial cell{q, r};
            const auto center = layout->tryCellCenter(cell);
            check(center && layout->tryCellAt(*center) == cell, "center round trip");
        }
    }

    const auto unit = PointyLayout::tryCreate(1);
    // Edge between origin and (0,1): fractional cube=(0,.5,-.5).
    const Point edge{std::numbers::sqrt3 / 4.0, 0.75};
    check(unit->tryCellAt(edge) == Axial{0, 1}, "computed r/s tie priority");
    check(unit->tryCellAt({-edge.x, -edge.y}) == Axial{0, -1}, "negative computed tie");
    check(unit->tryCellAt({edge.x * 1.000001, edge.y * 1.000001}) == Axial{0, 1}, "edge outside");
    check(unit->tryCellAt({edge.x * 0.999999, edge.y * 0.999999}) == Axial{0, 0}, "edge inside");
    // Vertex: only assert one of the three equally nearest cells, then stable repeat.
    const Point vertex{std::numbers::sqrt3 / 2.0, 0.5};
    const auto at_vertex = unit->tryCellAt(vertex);
    check(at_vertex &&
              (*at_vertex == Axial{} || *at_vertex == Axial{1, 0} || *at_vertex == Axial{0, 1}),
          "vertex nearest set");
    check(unit->tryCellAt(vertex) == at_vertex, "vertex stable repeat");

    // Euclidean nearest-center search is independent of cube-rounding branches.
    for (int ix = -17; ix <= 17; ++ix) {
        for (int iy = -17; iy <= 17; ++iy) {
            const Point point{ix * 0.231 + 0.019, iy * 0.197 + 0.037};
            Axial nearest{};
            double best = infinity;
            for (int q = -8; q <= 8; ++q) {
                for (int r = -8; r <= 8; ++r) {
                    const double cx = std::sqrt(3.0) * q + std::sqrt(3.0) * r / 2.0;
                    const double cy = 3.0 * r / 2.0;
                    const double squared =
                        (point.x - cx) * (point.x - cx) + (point.y - cy) * (point.y - cy);
                    if (squared < best) {
                        best = squared;
                        nearest = {q, r};
                    }
                }
            }
            check(unit->tryCellAt(point) == nearest, "independent nearest-center oracle");
        }
    }

    check(!unit->tryCellAt({nan, 0}) && !unit->tryCellAt({0, infinity}), "invalid point");
    check(!unit->tryCellAt({maximum, maximum}), "conversion overflow");
    const auto huge = PointyLayout::tryCreate(maximum);
    check(huge && !huge->tryCellCenter({1, 1}), "center overflow");
    const auto tiny = PointyLayout::tryCreate(std::numeric_limits<double>::min());
    check(tiny && !tiny->tryCellAt({maximum, 0}), "normalized overflow");
    const auto shifted = PointyLayout::tryCreate(1, {maximum, maximum});
    check(shifted && !shifted->tryCellAt({-maximum, -maximum}), "origin subtraction overflow");
    check(shifted && !shifted->tryCellCenter({1, 0}), "center precision collapse");
    constexpr auto low = std::numeric_limits<std::int32_t>::min();
    constexpr auto high = std::numeric_limits<std::int32_t>::max();
    for (const Axial cell :
         std::array<Axial, 4>{{{low, low}, {high, high}, {low, high}, {high, low}}}) {
        const auto center = unit->tryCellCenter(cell);
        check(center && unit->tryCellAt(*center) == cell, "extreme coordinate round trip");
    }
}
} // namespace

TEST_CASE("Pointy geometry agrees with independent nearest-center oracle") { checkGeometry(); }
