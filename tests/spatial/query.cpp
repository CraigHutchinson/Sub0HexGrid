#include <doctest/doctest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <vector>

#include "../../examples/spatial/SpatialIndex.hpp"

namespace {
using namespace sub0hexgrid;
using Index = example::SpatialIndex;

void Finish(Index& index, std::size_t budget) {
    auto state = index.StepRebuild(budget);
    while (state == Index::BuildState::working) state = index.StepRebuild(budget);
    REQUIRE(state == Index::BuildState::complete);
}

std::vector<std::size_t> Collect(Index::Query query, std::size_t budget) {
    std::vector<std::size_t> result;
    std::array<std::size_t, 3> rows{};
    for (;;) {
        const auto batch = query.Read(rows, budget);
        REQUIRE(batch.valid);
        REQUIRE(batch.work <= budget);
        result.insert(result.end(), rows.begin(), rows.begin() + static_cast<std::ptrdiff_t>(batch.written));
        if (batch.done) break;
    }
    std::ranges::sort(result);
    return result;
}
}

TEST_CASE("Spatial example queries match exact brute force across workload batches") {
    const auto layout = *PointyLayout::TryCreate(1.3, {2, -7});
    const auto region = *AxialRegion::TryCreate({-20, -20}, {20, 20});
    std::vector<Point> points;
    for (int y = -8; y <= 8; ++y)
        for (int x = -8; x <= 8; ++x) points.push_back({x * 0.73, y * 0.51});
    Index index{layout, region, points.size()};
    REQUIRE(index.BeginRebuild(points));
    CHECK(index.StepRebuild(0) == Index::BuildState::working);
    CHECK(index.GetSampleCount() == 0);
    CHECK_FALSE(index.BeginRebuild(points));
    Finish(index, 1);
    for (const Point center : std::array<Point, 4>{{{0, 0}, {-8, -8}, {30, 30}, {2, -7}}}) {
        for (const double radius : {0.0, 0.8, 3.7, 100.0}) {
            std::vector<std::size_t> expected;
            for (std::size_t row = 0; row < points.size(); ++row)
                if (std::hypot(points[row].x - center.x, points[row].y - center.y) <= radius)
                    expected.push_back(row);
            const auto query = index.TryQuery(center, radius);
            REQUIRE(query);
            auto paused = *query;
            std::array<std::size_t, 2> untouched{999, 999};
            const auto zero = paused.Read(untouched, 0);
            CHECK(zero.work == 0);
            CHECK(zero.written == 0);
            CHECK(untouched[0] == 999);
            const auto empty = paused.Read(std::span<std::size_t>{}, 10);
            CHECK(empty.work == 0);
            CHECK(empty.written == 0);
            CHECK(Collect(paused, 1) == expected);
            CHECK(Collect(*query, 7) == expected);
            CHECK(Collect(*query, 10000) == expected);
        }
    }
}

TEST_CASE("Dense-bin queries resume mid-bin and rebuild failures preserve committed state") {
    const auto layout = *PointyLayout::TryCreate(1);
    const auto region = *AxialRegion::TryCreate({-2, -2}, {2, 2});
    std::array<Point, 100> points{};
    Index index{layout, region, points.size()};
    REQUIRE(index.BeginRebuild(points));
    Finish(index, 7);
    auto query = *index.TryQuery({}, 0);
    std::array<std::size_t, 100> rows{};
    auto first = query.Read(rows, 13);
    CHECK(first.work <= 13);
    CHECK_FALSE(first.done);
    auto copy = query;
    CHECK(Collect(copy, 1) == Collect(query, 11));
    const std::array<Point, 1> invalid{{{std::numeric_limits<double>::quiet_NaN(), 0}}};
    REQUIRE(index.BeginRebuild(invalid));
    auto state = index.StepRebuild(1000);
    CHECK(state == Index::BuildState::failed);
    CHECK(index.GetSampleCount() == 100);
    CHECK(Collect(*index.TryQuery({}, 0), 3).size() == 100);
    REQUIRE(index.BeginRebuild(points));
    CHECK(index.StepRebuild(2) == Index::BuildState::working);
    index.CancelRebuild();
    CHECK(index.GetSampleCount() == 100);
    REQUIRE(index.BeginRebuild(std::span<const Point>{}));
    Finish(index, 1);
    CHECK(index.GetSampleCount() == 0);
    CHECK_FALSE(query.Read(rows, 1).valid);
    CHECK(Collect(*index.TryQuery({}, 0), 1).empty());
    CHECK_FALSE(index.TryQuery({}, -1));
    CHECK_FALSE(index.TryQuery({std::numeric_limits<double>::infinity(), 0}, 1));
}
