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

#include "sub0hexgrid/PointyLayout.hpp"

namespace {
void Check(bool condition, std::string_view message) {
    REQUIRE_MESSAGE(condition, message);
}

void CheckTopology() {
    using namespace sub0hexgrid;
    constexpr std::array directions{Direction::east, Direction::northeast, Direction::northwest,
                                    Direction::west, Direction::southwest, Direction::southeast};
    constexpr std::array<Axial, 6> expected{{{1,0},{1,-1},{0,-1},{-1,0},{-1,1},{0,1}}};
    for (std::size_t i = 0; i < directions.size(); ++i) {
        const auto neighbor = TryNeighbor({}, directions[i]);
        Check(neighbor == expected[i], "direction convention");
        Check(TryNeighbor(*neighbor, directions[(i + 3) % 6]) == Axial{}, "inverse direction");
        Check(ComputeDistance({}, *neighbor) == 1, "neighbor distance");
    }
    Check(!TryNeighbor({}, static_cast<Direction>(255)), "invalid direction");
    constexpr auto low = std::numeric_limits<std::int32_t>::min();
    constexpr auto high = std::numeric_limits<std::int32_t>::max();
    Check(!TryNeighbor({high,0}, Direction::east), "q positive overflow");
    Check(!TryNeighbor({low,0}, Direction::west), "q negative overflow");
    Check(!TryNeighbor({0,high}, Direction::southeast), "r positive overflow");
    Check(!TryNeighbor({0,low}, Direction::northwest), "r negative overflow");
    Check(ComputeDistance({low,low}, {high,high}) == 8589934590ULL, "full domain distance");
    Check(ComputeDistance({high,low}, {low,high}) == 4294967295ULL, "opposite axes distance");

    // A graph search with explicit edges is independent of the distance formula.
    std::map<Axial, std::uint64_t> oracle{{{}, 0}};
    std::queue<Axial> pending;
    pending.push({});
    while (!pending.empty()) {
        const auto cell = pending.front();
        pending.pop();
        if (oracle.at(cell) == 8) continue;
        for (const auto step : expected) {
            const Axial next{cell.q + step.q, cell.r + step.r};
            if (oracle.emplace(next, oracle.at(cell) + 1).second) pending.push(next);
        }
    }
    for (const auto& [cell, distance] : oracle) {
        Check(ComputeDistance({}, cell) == distance, "shortest path oracle");
        Check(ComputeDistance(cell, {}) == distance, "distance symmetry");
        Check(ComputeDistance(cell, cell) == 0, "distance identity");
        for (const auto& [other, other_distance] : oracle)
            Check(ComputeDistance(cell, other) <= distance + other_distance, "triangle inequality");
    }
}


}

TEST_CASE("Checked axial topology agrees with independent graph oracle") {
    CheckTopology();
}
