#include <sub0hexgrid/candidates/CandidateCells.hpp>
#include <sub0hexgrid/candidates/CandidateCursor.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

#include <doctest/doctest.h>

namespace {
using namespace sub0hexgrid;
static_assert(std::forward_iterator<CandidateCells::Iterator>);
static_assert(std::ranges::forward_range<CandidateCells>);
static_assert(std::ranges::forward_range<const CandidateCells>);
static_assert(std::is_trivially_copyable_v<CandidateCells>);
static_assert(std::is_trivially_copyable_v<CandidateCursor>);

std::vector<Axial> Collect(const CandidateCells& cells) {
    return {cells.begin(), cells.end()};
}

bool Contains(const CandidateCells& cells, Axial cell) {
    return std::ranges::find(cells, cell) != cells.end();
}

void CheckSample(const PointyLayout& layout, const AxialRegion& region,
    const CandidateCells& candidates, Point point) {
    const auto cell = layout.TryCellAt(point);
    if (cell && region.Contains(*cell)) CHECK(Contains(candidates, *cell));
}
}

TEST_CASE("candidate order and clipping preserve compact region membership") {
    const auto layout = PointyLayout::TryCreate(1.0);
    const auto region = AxialRegion::TryCreate({-5, -4}, {6, 7});
    REQUIRE(layout);
    REQUIRE(region);
    const auto cells = CandidateCells::TryCreate(*layout, *region, {}, 100.0);
    REQUIRE(cells);
    CHECK(cells->GetCellCount() == region->GetCellCount());
    std::uint64_t index = 0;
    for (const auto cell : *cells) {
        CHECK(region->TryCell(index) == cell);
        CHECK(region->Contains(cell));
        ++index;
    }
    CHECK(index == cells->GetCellCount());
    auto iterator = cells->begin();
    const auto original = iterator;
    CHECK(*iterator++ == *original);
    CHECK(original == cells->begin());
    CHECK(iterator != original);

    const auto disjoint = CandidateCells::TryCreate(*layout, *region, {1000, 1000}, 0.0);
    REQUIRE(disjoint);
    CHECK(disjoint->GetCellCount() == 0);
    CHECK(disjoint->begin() == disjoint->end());
    const CandidateCursor emptyCursor{*disjoint};
    CHECK(emptyCursor.IsDone());
    CHECK(emptyCursor.Remaining() == 0);
}

TEST_CASE("candidate slices partition without gaps and survive source lifetime") {
    const auto layout = PointyLayout::TryCreate(1.0);
    const auto region = AxialRegion::TryCreate({-7, -4}, {7, 4});
    REQUIRE(layout);
    REQUIRE(region);
    const auto cells = CandidateCells::TryCreate(*layout, *region, {}, 5.0);
    REQUIRE(cells);
    const auto full = Collect(*cells);
    std::vector<Axial> partitioned;
    for (std::uint64_t offset = 0; offset < cells->GetCellCount();) {
        const auto count = std::min<std::uint64_t>(7, cells->GetCellCount() - offset);
        const auto slice = cells->TrySlice(offset, count);
        REQUIRE(slice);
        partitioned.insert(partitioned.end(), slice->begin(), slice->end());
        offset += count;
    }
    CHECK(partitioned == full);
    const auto endSlice = cells->TrySlice(cells->GetCellCount(), 0);
    REQUIRE(endSlice);
    CHECK(endSlice->begin() == endSlice->end());
    CHECK_FALSE(cells->TrySlice(cells->GetCellCount(), 1));
    CHECK_FALSE(cells->TrySlice(cells->GetCellCount() + 1, 0));
    CHECK_FALSE(cells->TrySlice(1, std::numeric_limits<std::uint64_t>::max()));
    const auto firstSlice = cells->TrySlice(2, 10);
    REQUIRE(firstSlice);
    const auto nested = firstSlice->TrySlice(3, 4);
    REQUIRE(nested);
    CHECK(Collect(*nested) == std::vector<Axial>(full.begin() + 5, full.begin() + 9));

    const auto ownedIterator = [&] {
        const auto temporary = cells->TrySlice(3, 2);
        return temporary->begin();
    }();
    CHECK(*ownedIterator == full[3]);
}

TEST_CASE("candidate cursor yields mid row and copies snapshot progress") {
    const auto layout = PointyLayout::TryCreate(1.0);
    const auto region = AxialRegion::TryCreate({-5, -4}, {6, 7});
    REQUIRE(layout);
    REQUIRE(region);
    const auto cells = CandidateCells::TryCreate(*layout, *region, {}, 100.0);
    REQUIRE(cells);
    const auto expected = Collect(*cells);
    for (const std::size_t capacity : {std::size_t{1}, std::size_t{7}, std::size_t{19}}) {
        CAPTURE(capacity);
        CandidateCursor cursor{*cells};
        CHECK(cursor.Read({}) == 0);
        CHECK(cursor.Remaining() == cells->GetCellCount());
        CHECK_FALSE(cursor.IsDone());
        std::vector<Axial> collected;
        std::array<Axial, 19> buffer{};
        while (!cursor.IsDone()) {
            const auto written = cursor.Read(std::span{buffer}.first(capacity));
            REQUIRE(written != 0);
            collected.insert(collected.end(), buffer.begin(), buffer.begin() +
                static_cast<std::ptrdiff_t>(written));
            CHECK(cursor.Remaining() == cells->GetCellCount() - collected.size());
            auto snapshot = cursor;
            std::array<Axial, 7> left{}, right{};
            auto independentlyAdvanced = cursor;
            const auto leftCount = snapshot.Read(left);
            const auto rightCount = independentlyAdvanced.Read(right);
            CHECK(leftCount == rightCount);
            CHECK(left == right);
            CHECK(snapshot.Remaining() == independentlyAdvanced.Remaining());
        }
        CHECK(collected == expected);
        buffer.fill({123, 456});
        CHECK(cursor.Read(buffer) == 0);
        CHECK(buffer.front() == Axial{123, 456});
    }
}

TEST_CASE("candidate completeness against independently sampled inclusive world disks") {
    const auto region = AxialRegion::TryCreate({-20, -20}, {20, 20});
    REQUIRE(region);
    for (const double scale : {1e-100, 0.125, 1.0, 17.0, 1e100}) {
        const Point origin{scale * 11.0, -scale * 7.0};
        const auto layout = PointyLayout::TryCreate(scale, origin);
        REQUIRE(layout);
        for (const Point fractionalCenter : {Point{}, Point{0.5, -1.5}, Point{-3.25, 2.75}}) {
            const Point center{origin.x + scale * fractionalCenter.x,
                origin.y + scale * fractionalCenter.y};
            for (const double radiusFactor : {0.0, 0.1, 1.0, 3.75}) {
                CAPTURE(scale);
                CAPTURE(fractionalCenter.x);
                CAPTURE(fractionalCenter.y);
                CAPTURE(radiusFactor);
                const double radius = scale * radiusFactor;
                const auto candidates = CandidateCells::TryCreate(*layout, *region, center, radius);
                REQUIRE(candidates);
                CheckSample(*layout, *region, *candidates, center);
                for (int iy = -12; iy <= 12; ++iy) {
                    for (int ix = -12; ix <= 12; ++ix) {
                        // Integer disk membership is independent of production axial bounds.
                        if (ix * ix + iy * iy > 144) continue;
                        const Point point{center.x + radius * (ix / 12.0),
                            center.y + radius * (iy / 12.0)};
                        CheckSample(*layout, *region, *candidates, point);
                    }
                }
                for (int direction = 0; direction < 32; ++direction) {
                    const double angle = direction * (2.0 * std::numbers::pi / 32.0);
                    const Point point{center.x + radius * std::cos(angle),
                        center.y + radius * std::sin(angle)};
                    CheckSample(*layout, *region, *candidates, point);
                }
            }
        }
    }
}

TEST_CASE("candidate rounding envelope includes seams and int32 region edges") {
    const auto layout = PointyLayout::TryCreate(1.0);
    const auto region = AxialRegion::TryCreate({-12, -12}, {12, 12});
    REQUIRE(layout);
    REQUIRE(region);
    const double seamX = std::numbers::sqrt3 / 2.0;
    const double infinity = std::numeric_limits<double>::infinity();
    for (const double x : {seamX, std::nextafter(seamX, -infinity),
            std::nextafter(seamX, infinity), -seamX}) {
        for (const double y : {0.0, 0.5, -0.5, 1.0, -1.0}) {
            const Point center{x, y};
            const auto candidates = CandidateCells::TryCreate(*layout, *region, center, 0.0);
            REQUIRE(candidates);
            const auto cell = layout->TryCellAt(center);
            REQUIRE(cell);
            CHECK(Contains(*candidates, *cell));
        }
    }

    constexpr auto minimum = std::numeric_limits<std::int32_t>::min();
    constexpr auto maximum = std::numeric_limits<std::int32_t>::max();
    for (const Axial edge : {Axial{minimum, 0}, Axial{maximum, 0},
            Axial{0, minimum}, Axial{0, maximum}}) {
        const auto edgeRegion = AxialRegion::TryCreate(edge, edge);
        const auto center = layout->TryCellCenter(edge);
        REQUIRE(edgeRegion);
        REQUIRE(center);
        const auto candidates = CandidateCells::TryCreate(*layout, *edgeRegion, *center, 0.0);
        REQUIRE(candidates);
        CHECK(candidates->GetCellCount() == 1);
        CHECK(*candidates->begin() == edge);
    }
}

TEST_CASE("candidate invalid arithmetic fails before traversal") {
    const auto layout = PointyLayout::TryCreate(1.0);
    const auto region = AxialRegion::TryCreate({-2, -2}, {2, 2});
    REQUIRE(layout);
    REQUIRE(region);
    const double infinity = std::numeric_limits<double>::infinity();
    const double nan = std::numeric_limits<double>::quiet_NaN();
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region, {}, -1.0));
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region, {}, infinity));
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region, {}, nan));
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region, {infinity, 0}, 1));
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region, {0, nan}, 1));
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region, {}, 0x1p43));
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region, {0x1p43, 0}, 0));
    CHECK_FALSE(CandidateCells::TryCreate(*layout, *region,
        {std::numeric_limits<double>::max(), 0}, std::numeric_limits<double>::max()));
    const auto tiny = PointyLayout::TryCreate(std::numeric_limits<double>::denorm_min());
    REQUIRE(tiny);
    CHECK_FALSE(CandidateCells::TryCreate(*tiny, *region, {}, 1));
    const auto translated = PointyLayout::TryCreate(1, {std::numeric_limits<double>::max(), 0});
    REQUIRE(translated);
    CHECK_FALSE(CandidateCells::TryCreate(*translated, *region,
        {-std::numeric_limits<double>::max(), 0}, 0));
}

TEST_CASE("candidate counts and end slices support more than signed iterator distance") {
    constexpr auto minimum = std::numeric_limits<std::int32_t>::min();
    constexpr auto maximum = std::numeric_limits<std::int32_t>::max();
    const auto region = AxialRegion::TryCreate({minimum, minimum}, {maximum, maximum - 1});
    const auto layout = PointyLayout::TryCreate(1.0);
    REQUIRE(region);
    REQUIRE(layout);
    const auto candidates = CandidateCells::TryCreate(*layout, *region, {}, 1e10);
    REQUIRE(candidates);
    CHECK(candidates->GetCellCount() == region->GetCellCount());
    CHECK(candidates->GetCellCount() > static_cast<std::uint64_t>(
        std::numeric_limits<std::int64_t>::max()));
    const auto tail = candidates->TrySlice(candidates->GetCellCount() - 2, 2);
    REQUIRE(tail);
    CandidateCursor cursor{*tail};
    std::array<Axial, 3> buffer{{{}, {}, {17, 23}}};
    CHECK(cursor.Read(buffer) == 2);
    CHECK(buffer[0] == Axial{maximum - 1, maximum - 1});
    CHECK(buffer[1] == Axial{maximum, maximum - 1});
    CHECK(buffer[2] == Axial{17, 23});
    CHECK(cursor.IsDone());
    const auto emptyTail = candidates->TrySlice(candidates->GetCellCount(), 0);
    REQUIRE(emptyTail);
    CHECK(emptyTail->begin() == candidates->end());
    CHECK_FALSE(emptyTail->TrySlice(0, 1));
}
