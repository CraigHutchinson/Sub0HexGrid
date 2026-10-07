#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <nanobench.h>
#include "../examples/spatial/SpatialIndex.hpp"

namespace
{
using namespace sub0hexgrid;
using Index = example::SpatialIndex;
using Clock = std::chrono::steady_clock;

struct Counts
{
    std::size_t hits_{};
    std::uint64_t work_{};
};
Counts query(const Index& index, Point center, double radius)
{
    auto query = index.tryQuery(center, radius);
    if (!query)
        std::abort();
    std::array<std::size_t, 256> output{};
    Counts counts{};
    for (;;) {
        const auto batch = query->read(output, 4096);
        if (!batch.valid_)
            std::abort();
        counts.hits_ += batch.written_;
        counts.work_ += batch.work_;
        ankerl::nanobench::doNotOptimizeAway(output);
        if (batch.done_)
            return counts;
    }
}

void rebuild(Index& index, std::span<const Point> points)
{
    if (!index.beginRebuild(points))
        std::abort();
    auto state = index.stepRebuild(4096);
    while (state == Index::BuildState::working)
        state = index.stepRebuild(4096);
    if (state != Index::BuildState::complete)
        std::abort();
}

void measure(std::size_t cells, std::size_t samples, std::string_view distribution, bool smoke,
             bool large = false)
{
    const auto layout = *PointyLayout::tryCreate(1.0);
    const auto height = static_cast<std::int32_t>(cells / 1000);
    const auto region =
        *AxialRegion::tryCreate({-500, -height / 2}, {499, height - height / 2 - 1});
    std::vector<Point> points(samples);
    std::vector<std::size_t> occupancy(cells);
    for (std::size_t i = 0; i < samples; ++i) {
        Axial cell{};
        if (distribution == "coincident") {
            cell = {};
        } else if (distribution == "clustered") {
            cell = {static_cast<std::int32_t>(i % 64) - 32,
                    static_cast<std::int32_t>((i / 64) % 64) - 32};
        } else if (distribution == "border") {
            cell = {i % 2 == 0 ? -500 : 499,
                    static_cast<std::int32_t>((i / 2) % static_cast<std::size_t>(height)) -
                        height / 2};
        } else {
            cell = *region.tryCell((i * 7919) % cells);
        }
        points[i] = *layout.tryCellCenter(cell);
        ++occupancy[static_cast<std::size_t>(*region.tryIndex(cell))];
    }
    Index index{layout, region, samples};
    rebuild(index, points);
    const auto query_count = smoke || samples == 0 || large || distribution == "coincident"
                                 ? std::size_t{64}
                                 : std::size_t{4096};
    std::vector<Point> centers(query_count);
    for (std::size_t i = 0; i < centers.size(); ++i) {
        const auto point = samples == 0 ? *layout.tryCellCenter(*region.tryCell(i % cells))
                                        : points[(i * 104729) % samples];
        centers[i] = {point.x + 0.17, point.y - 0.23};
    }
    std::vector<double> latency(query_count);
    const std::string title =
        std::to_string(samples) + "/" + std::string(distribution) + (large ? "/large" : "");
    std::cout << "\nWORKLOAD " << title << " cells=" << cells
              << " region_bytes=" << sizeof(AxialRegion)
              << " resident_index_payload_bytes=" << index.getStorageBytes()
              << " caller_points_bytes=" << samples * sizeof(Point) << " nonempty_cells="
              << std::ranges::count_if(occupancy, [](auto count) { return count != 0; })
              << " max_occupancy=" << *std::ranges::max_element(occupancy) << '\n';
    ankerl::nanobench::Bench bench;
    bench.title(title).epochs(smoke ? 1 : 3).minEpochIterations(1);
    bench.unit("sample").batch(std::max(samples, std::size_t{1})).run("mapping", [&] {
        for (auto point : points)
            ankerl::nanobench::doNotOptimizeAway(layout.tryCellAt(point));
    });
    bench.unit("sample").batch(std::max(samples, std::size_t{1})).run("bounded rebuild", [&] {
        rebuild(index, points);
    });
    bench.run("exact distance predicate", [&] {
        for (auto point : points)
            ankerl::nanobench::doNotOptimizeAway(
                std::hypot(point.x - centers[0].x, point.y - centers[0].y) <= 2.0);
    });
    bench.unit("cell").batch(cells).run("region index roundtrip", [&] {
        for (std::uint64_t i = 0; i < cells; ++i)
            ankerl::nanobench::doNotOptimizeAway(region.tryIndex(*region.tryCell(i)));
    });
    const std::vector<double> radii =
        large ? std::vector<double>{1000.0} : std::vector<double>{2.0, 8.0};
    for (double radius : radii) {
        std::uint64_t candidates = 0;
        std::size_t hits = 0;
        std::uint64_t work = 0;
        for (std::size_t i = 0; i < centers.size(); ++i) {
            candidates +=
                CandidateCells::tryCreate(layout, region, centers[i], radius)->getCellCount();
            const auto start = Clock::now();
            const auto result = query(index, centers[i], radius);
            hits += result.hits_;
            work += result.work_;
            latency[i] = std::chrono::duration<double, std::micro>(Clock::now() - start).count();
        }
        std::ranges::sort(latency);
        std::cout << "QUERY radius=" << radius << " count=" << query_count
                  << " candidates=" << candidates << " hits=" << hits
                  << " examined=" << work - candidates << " rejected=" << work - candidates - hits
                  << " p95_us=" << latency[(query_count * 95 - 1) / 100]
                  << " p99_us=" << latency[(query_count * 99 - 1) / 100] << '\n';
        const auto suffix = std::to_string(static_cast<int>(radius));
        bench.unit("query").batch(query_count).run("candidate traversal r=" + suffix, [&] {
            for (auto center : centers) {
                const auto range = *CandidateCells::tryCreate(layout, region, center, radius);
                for (auto cell : range)
                    ankerl::nanobench::doNotOptimizeAway(cell);
            }
        });
        bench.run("combined exact query r=" + suffix, [&] {
            for (auto center : centers)
                ankerl::nanobench::doNotOptimizeAway(query(index, center, radius));
        });
    }
}
} // namespace

int main(int argc, char** argv)
{
    const bool smoke = argc == 2 && std::string_view{argv[1]} == "--smoke";
    if (argc > 1 && !smoke)
        return 1;
    std::cout << "H2 baseline: setup excluded; warm sequential queries; row-prefix consumption "
                 "included.\n";
    for (const auto cells : {std::size_t{100000}, std::size_t{500000}, std::size_t{1000000}})
        for (const auto distribution : {"uniform", "clustered", "border"})
            measure(cells, smoke ? std::size_t{2048} : cells, distribution, smoke);
    measure(100000, smoke ? std::size_t{2048} : std::size_t{1000000}, "coincident", smoke);
    measure(100000, 0, "empty", smoke);
    measure(100000, smoke ? std::size_t{2048} : std::size_t{1000000}, "uniform", smoke, true);
}
