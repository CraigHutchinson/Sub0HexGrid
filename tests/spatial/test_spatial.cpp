#include <doctest/doctest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <vector>

#include "../../examples/spatial/SpatialIndex.hpp"

namespace
{
using namespace sub0hexgrid;
using Index = example::SpatialIndex;

void finish(Index& index, std::size_t budget)
{
    auto state = index.stepRebuild(budget);
    while (state == Index::BuildState::working)
        state = index.stepRebuild(budget);
    REQUIRE(state == Index::BuildState::complete);
}

std::vector<std::size_t> collect(Index::Query query, std::size_t budget)
{
    std::vector<std::size_t> result;
    std::array<std::size_t, 3> rows{};
    for (;;) {
        const auto batch = query.read(rows, budget);
        REQUIRE(batch.valid_);
        REQUIRE(batch.work_ <= budget);
        result.insert(result.end(), rows.begin(),
                      rows.begin() + static_cast<std::ptrdiff_t>(batch.written_));
        if (batch.done_)
            break;
    }
    std::ranges::sort(result);
    return result;
}
} // namespace

TEST_CASE("Spatial example queries match exact brute force across workload batches")
{
    const auto layout = *PointyLayout::tryCreate(1.3, {2, -7});
    const auto region = *AxialRegion::tryCreate({-20, -20}, {20, 20});
    std::vector<Point> points;
    for (int y = -8; y <= 8; ++y)
        for (int x = -8; x <= 8; ++x)
            points.push_back({x * 0.73, y * 0.51});
    Index index{layout, region, points.size()};
    REQUIRE(index.beginRebuild(points));
    CHECK(index.stepRebuild(0) == Index::BuildState::working);
    CHECK(index.getSampleCount() == 0);
    CHECK_FALSE(index.beginRebuild(points));
    finish(index, 1);
    for (const Point center : std::array<Point, 4>{{{0, 0}, {-8, -8}, {30, 30}, {2, -7}}}) {
        for (const double radius : {0.0, 0.8, 3.7, 100.0}) {
            std::vector<std::size_t> expected;
            for (std::size_t row = 0; row < points.size(); ++row)
                if (std::hypot(points[row].x - center.x, points[row].y - center.y) <= radius)
                    expected.push_back(row);
            const auto query = index.tryQuery(center, radius);
            REQUIRE(query);
            auto paused = *query;
            std::array<std::size_t, 2> untouched{999, 999};
            const auto zero = paused.read(untouched, 0);
            CHECK(zero.work_ == 0);
            CHECK(zero.written_ == 0);
            CHECK(untouched[0] == 999);
            const auto empty = paused.read(std::span<std::size_t>{}, 10);
            CHECK(empty.work_ == 0);
            CHECK(empty.written_ == 0);
            CHECK(collect(paused, 1) == expected);
            CHECK(collect(*query, 7) == expected);
            CHECK(collect(*query, 10000) == expected);
        }
    }
}

TEST_CASE("Dense-bin queries resume mid-bin and rebuild failures preserve committed state")
{
    const auto layout = *PointyLayout::tryCreate(1);
    const auto region = *AxialRegion::tryCreate({-2, -2}, {2, 2});
    std::array<Point, 100> points{};
    Index index{layout, region, points.size()};
    REQUIRE(index.beginRebuild(points));
    finish(index, 7);
    auto query = *index.tryQuery({}, 0);
    std::array<std::size_t, 100> rows{};
    auto first = query.read(rows, 13);
    CHECK(first.work_ <= 13);
    CHECK_FALSE(first.done_);
    auto copy = query;
    CHECK(collect(copy, 1) == collect(query, 11));
    const std::array<Point, 1> invalid{{{std::numeric_limits<double>::quiet_NaN(), 0}}};
    REQUIRE(index.beginRebuild(invalid));
    auto state = index.stepRebuild(1000);
    CHECK(state == Index::BuildState::failed);
    CHECK(index.getSampleCount() == 100);
    CHECK(collect(*index.tryQuery({}, 0), 3).size() == 100);
    REQUIRE(index.beginRebuild(points));
    CHECK(index.stepRebuild(2) == Index::BuildState::working);
    index.cancelRebuild();
    CHECK(index.getSampleCount() == 100);
    REQUIRE(index.beginRebuild(std::span<const Point>{}));
    finish(index, 1);
    CHECK(index.getSampleCount() == 0);
    CHECK_FALSE(query.read(rows, 1).valid_);
    CHECK(collect(*index.tryQuery({}, 0), 1).empty());
    CHECK_FALSE(index.tryQuery({}, -1));
    CHECK_FALSE(index.tryQuery({std::numeric_limits<double>::infinity(), 0}, 1));
}
