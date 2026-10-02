#include <sub0hexgrid/regions/AxialRegion.hpp>

#include <array>
#include <cstdint>
#include <limits>
#include <type_traits>

#include <doctest/doctest.h>

using sub0hexgrid::Axial;
using sub0hexgrid::AxialRegion;

static_assert(!std::is_default_constructible_v<AxialRegion>);
static_assert(std::is_trivially_copyable_v<AxialRegion>);
static_assert(noexcept(AxialRegion::TryCreate({}, {})));

TEST_CASE("regions reject reversed inclusive bounds") {
    CHECK_FALSE(AxialRegion::TryCreate({1, 0}, {0, 0}));
    CHECK_FALSE(AxialRegion::TryCreate({0, 1}, {0, 0}));
    CHECK_FALSE(AxialRegion::TryCreate({1, 1}, {0, 0}));
}

TEST_CASE("regions use explicit r-outer q-inner order") {
    const auto region = AxialRegion::TryCreate({-2, -1}, {0, 1});
    REQUIRE(region);
    CHECK((region->GetMinimum() == Axial{-2, -1}));
    CHECK((region->GetMaximum() == Axial{0, 1}));
    constexpr std::array<Axial, 9> expected{{
        {-2, -1}, {-1, -1}, {0, -1}, {-2, 0}, {-1, 0}, {0, 0}, {-2, 1}, {-1, 1}, {0, 1}
    }};
    CHECK(region->GetCellCount() == expected.size());
    std::uint64_t index = 0;
    for (const auto cell : expected) {
        CHECK(region->Contains(cell));
        CHECK(region->TryIndex(cell) == index);
        CHECK(region->TryCell(index) == cell);
        ++index;
    }
    constexpr std::array<Axial, 4> outside{{{-3, 0}, {1, 0}, {-1, -2}, {-1, 2}}};
    for (const auto cell : outside) {
        CHECK_FALSE(region->Contains(cell));
        CHECK_FALSE(region->TryIndex(cell));
    }
    CHECK_FALSE(region->TryCell(expected.size()));
    CHECK_FALSE(region->TryCell(std::numeric_limits<std::uint64_t>::max()));
}

TEST_CASE("all small rectangles are bijections with independent enumeration") {
    // Enumeration counts cells directly, without the production index formula.
    for (std::int32_t minimumR = -2; minimumR <= 2; ++minimumR) {
        for (std::int32_t maximumR = minimumR; maximumR <= 2; ++maximumR) {
            for (std::int32_t minimumQ = -2; minimumQ <= 2; ++minimumQ) {
                for (std::int32_t maximumQ = minimumQ; maximumQ <= 2; ++maximumQ) {
                    const auto region = AxialRegion::TryCreate({minimumQ, minimumR}, {maximumQ, maximumR});
                    REQUIRE(region);
                    std::uint64_t index = 0;
                    for (auto r = minimumR; r <= maximumR; ++r) {
                        for (auto q = minimumQ; q <= maximumQ; ++q) {
                            const Axial cell{q, r};
                            CHECK(region->Contains(cell));
                            CHECK(region->TryIndex(cell) == index);
                            CHECK(region->TryCell(index) == cell);
                            ++index;
                        }
                    }
                    CHECK(region->GetCellCount() == index);
                    CHECK_FALSE(region->TryCell(index));
                }
            }
        }
    }
}

TEST_CASE("single cells and full-width thin regions retain extreme coordinates") {
    constexpr auto low = std::numeric_limits<std::int32_t>::min();
    constexpr auto high = std::numeric_limits<std::int32_t>::max();
    constexpr std::array<Axial, 5> singles{{{low, low}, {high, high}, {low, high}, {high, low}, {-7, -9}}};
    for (const auto cell : singles) {
        const auto region = AxialRegion::TryCreate(cell, cell);
        REQUIRE(region);
        CHECK(region->GetCellCount() == 1);
        CHECK(region->TryIndex(cell) == 0);
        CHECK(region->TryCell(0) == cell);
        CHECK_FALSE(region->TryCell(1));
    }

    constexpr std::uint64_t axisCount = std::uint64_t{1} << 32;
    const auto row = AxialRegion::TryCreate({low, high}, {high, high});
    const auto column = AxialRegion::TryCreate({low, low}, {low, high});
    REQUIRE(row);
    REQUIRE(column);
    CHECK(row->GetCellCount() == axisCount);
    CHECK(column->GetCellCount() == axisCount);
    CHECK((row->TryCell(axisCount - 1) == Axial{high, high}));
    CHECK((column->TryCell(axisCount - 1) == Axial{low, high}));
    CHECK(row->TryIndex({high, high}) == axisCount - 1);
    CHECK(column->TryIndex({low, high}) == axisCount - 1);
    CHECK_FALSE(row->TryCell(axisCount));
    CHECK_FALSE(column->TryCell(axisCount));
}

TEST_CASE("region count rejects uint64 overflow but accepts neighboring huge regions") {
    constexpr auto low = std::numeric_limits<std::int32_t>::min();
    constexpr auto high = std::numeric_limits<std::int32_t>::max();
    CHECK_FALSE(AxialRegion::TryCreate({low, low}, {high, high}));

    constexpr std::uint64_t axisCount = std::uint64_t{1} << 32;
    constexpr auto expectedCount = axisCount * (axisCount - 1);
    const auto region = AxialRegion::TryCreate({low, low}, {high, high - 1});
    REQUIRE(region);
    CHECK(region->GetCellCount() == expectedCount);
    CHECK((region->TryCell(0) == Axial{low, low}));
    CHECK((region->TryCell(axisCount - 1) == Axial{high, low}));
    CHECK((region->TryCell(axisCount) == Axial{low, low + 1}));
    CHECK((region->TryCell(expectedCount - 1) == Axial{high, high - 1}));
    CHECK(region->TryIndex({high, high - 1}) == expectedCount - 1);
    CHECK_FALSE(region->Contains({high, high}));
    CHECK_FALSE(region->TryIndex({high, high}));
    CHECK_FALSE(region->TryCell(expectedCount));
    CHECK_FALSE(region->TryCell(std::numeric_limits<std::uint64_t>::max()));
}
