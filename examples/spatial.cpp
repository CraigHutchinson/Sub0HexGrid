#include "spatial/SpatialIndex.hpp"

#include <array>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iostream>
#include <vector>

int main()
{
    using namespace sub0hexgrid;
    const auto layout = PointyLayout::tryCreate(2.0, {10.0, 20.0});
    if (!layout)
        return 1;
    // The world square's conservative mapped cover is the same checked query envelope.
    const auto domain = AxialRegion::tryCreate({-1000, -1000}, {1000, 1000});
    if (!domain)
        return 2;
    const auto cover = CandidateCells::tryCreate(*layout, *domain, {10, 20}, 40);
    if (!cover || cover->getCellCount() == 0)
        return 3;
    const auto first = *cover->begin();
    const Axial maximum = *cover->trySlice(cover->getCellCount() - 1, 1)->begin();
    const auto region = AxialRegion::tryCreate(first, maximum);
    if (!region)
        return 4;
    std::array<Point, 256> points{};
    for (std::size_t i = 0; i < points.size(); ++i)
        points[i] = {10.0 + static_cast<double>(i % 16) - 7.5,
                     20.0 + static_cast<double>(i / 16) - 7.5};
    example::SpatialIndex index{*layout, *region, points.size()};
    if (!index.beginRebuild(points))
        return 5;
    while (index.stepRebuild(64) == example::SpatialIndex::BuildState::working) {
    }
    if (index.getSampleCount() != points.size())
        return 6;
    const Point center{10.25, 20.5};
    constexpr double radius = 5;
    auto query = index.tryQuery(center, radius);
    if (!query)
        return 7;
    std::vector<std::size_t> hits;
    hits.reserve(points.size());
    std::array<std::size_t, 8> output{};
    bool complete = false;
    std::size_t slices = 0;
    while (!complete) {
        const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds{1};
        do {
            const auto batch = query->read(output, 32);
            if (!batch.valid_)
                return 8;
            hits.insert(hits.end(), output.begin(),
                        output.begin() + static_cast<std::ptrdiff_t>(batch.written_));
            complete = batch.done_;
        } while (!complete && std::chrono::steady_clock::now() < deadline);
        ++slices;
        // A real game returns to its scheduler here, retaining query and caller output.
    }
    std::vector<std::size_t> expected;
    for (std::size_t row = 0; row < points.size(); ++row)
        if (std::hypot(points[row].x - center.x, points[row].y - center.y) <= radius)
            expected.push_back(row);
    std::ranges::sort(hits);
    if (hits != expected)
        return 9;
    std::cout << "Spatial example: " << points.size() << " samples, " << hits.size()
              << " exact hits; " << slices << " deadline slices; caller-owned bins/cursors\n";
}
