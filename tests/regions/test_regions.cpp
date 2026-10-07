#include "sub0hexgrid/regions/axial_region.hpp"

#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

#include <doctest/doctest.h>

using sub0hexgrid::Axial;
using sub0hexgrid::AxialRegion;

static_assert(!std::is_default_constructible_v<AxialRegion>);
static_assert(std::is_trivially_copyable_v<AxialRegion>);
static_assert(noexcept(AxialRegion::tryCreate({}, {})));

TEST_CASE("regions reject reversed inclusive bounds")
{
    CHECK_FALSE(AxialRegion::tryCreate({1, 0}, {0, 0}));
    CHECK_FALSE(AxialRegion::tryCreate({0, 1}, {0, 0}));
    CHECK_FALSE(AxialRegion::tryCreate({1, 1}, {0, 0}));
}

TEST_CASE("regions use explicit r-outer q-inner order")
{
    const auto region = AxialRegion::tryCreate({-2, -1}, {0, 1});
    REQUIRE(region);
    CHECK((region->getMinimum() == Axial{-2, -1}));
    CHECK((region->getMaximum() == Axial{0, 1}));
    constexpr std::array<Axial, 9> expected{
        {{-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {-2, 1}, {-1, 1}, {0, 1}}};
    CHECK(region->getCellCount() == expected.size());
    std::uint64_t index = 0;
    for (const auto cell : expected) {
        CHECK(region->contains(cell));
        CHECK(region->tryIndex(cell) == index);
        CHECK(region->tryCell(index) == cell);
        ++index;
    }
    constexpr std::array<Axial, 4> outside{{{-3, 0}, {1, 0}, {-1, -2}, {-1, 2}}};
    for (const auto cell : outside) {
        CHECK_FALSE(region->contains(cell));
        CHECK_FALSE(region->tryIndex(cell));
    }
    CHECK_FALSE(region->tryCell(expected.size()));
    CHECK_FALSE(region->tryCell(std::numeric_limits<std::uint64_t>::max()));
}

TEST_CASE("all small rectangles are bijections with independent enumeration")
{
    // Enumeration counts cells directly, without the production index formula.
    for (std::int32_t minimumR = -2; minimumR <= 2; ++minimumR) {
        for (std::int32_t maximumR = minimumR; maximumR <= 2; ++maximumR) {
            for (std::int32_t minimumQ = -2; minimumQ <= 2; ++minimumQ) {
                for (std::int32_t maximumQ = minimumQ; maximumQ <= 2; ++maximumQ) {
                    const auto region =
                        AxialRegion::tryCreate({minimumQ, minimumR}, {maximumQ, maximumR});
                    REQUIRE(region);
                    std::uint64_t index = 0;
                    for (auto r = minimumR; r <= maximumR; ++r) {
                        for (auto q = minimumQ; q <= maximumQ; ++q) {
                            const Axial cell{q, r};
                            CHECK(region->contains(cell));
                            CHECK(region->tryIndex(cell) == index);
                            CHECK(region->tryCell(index) == cell);
                            ++index;
                        }
                    }
                    CHECK(region->getCellCount() == index);
                    CHECK_FALSE(region->tryCell(index));
                }
            }
        }
    }
}

TEST_CASE("single cells and full-width thin regions retain extreme coordinates")
{
    constexpr auto low = std::numeric_limits<std::int32_t>::min();
    constexpr auto high = std::numeric_limits<std::int32_t>::max();
    constexpr std::array<Axial, 5> singles{
        {{low, low}, {high, high}, {low, high}, {high, low}, {-7, -9}}};
    for (const auto cell : singles) {
        const auto region = AxialRegion::tryCreate(cell, cell);
        REQUIRE(region);
        CHECK(region->getCellCount() == 1);
        CHECK(region->tryIndex(cell) == 0);
        CHECK(region->tryCell(0) == cell);
        CHECK_FALSE(region->tryCell(1));
    }

    constexpr std::uint64_t axisCount = std::uint64_t{1} << 32;
    const auto row = AxialRegion::tryCreate({low, high}, {high, high});
    const auto column = AxialRegion::tryCreate({low, low}, {low, high});
    REQUIRE(row);
    REQUIRE(column);
    CHECK(row->getCellCount() == axisCount);
    CHECK(column->getCellCount() == axisCount);
    CHECK((row->tryCell(axisCount - 1) == Axial{high, high}));
    CHECK((column->tryCell(axisCount - 1) == Axial{low, high}));
    CHECK(row->tryIndex({high, high}) == axisCount - 1);
    CHECK(column->tryIndex({low, high}) == axisCount - 1);
    CHECK_FALSE(row->tryCell(axisCount));
    CHECK_FALSE(column->tryCell(axisCount));
}

TEST_CASE("region count rejects uint64 overflow but accepts neighboring huge regions")
{
    constexpr auto low = std::numeric_limits<std::int32_t>::min();
    constexpr auto high = std::numeric_limits<std::int32_t>::max();
    CHECK_FALSE(AxialRegion::tryCreate({low, low}, {high, high}));

    constexpr std::uint64_t axisCount = std::uint64_t{1} << 32;
    constexpr auto expectedCount = axisCount * (axisCount - 1);
    const auto region = AxialRegion::tryCreate({low, low}, {high, high - 1});
    REQUIRE(region);
    CHECK(region->getCellCount() == expectedCount);
    CHECK((region->tryCell(0) == Axial{low, low}));
    CHECK((region->tryCell(axisCount - 1) == Axial{high, low}));
    CHECK((region->tryCell(axisCount) == Axial{low, low + 1}));
    CHECK((region->tryCell(expectedCount - 1) == Axial{high, high - 1}));
    CHECK(region->tryIndex({high, high - 1}) == expectedCount - 1);
    CHECK_FALSE(region->contains({high, high}));
    CHECK_FALSE(region->tryIndex({high, high}));
    CHECK_FALSE(region->tryCell(expectedCount));
    CHECK_FALSE(region->tryCell(std::numeric_limits<std::uint64_t>::max()));
}
