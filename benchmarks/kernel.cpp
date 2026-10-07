#include <array>
#include <cstddef>
#include <iostream>

#include <nanobench.h>
#include "sub0hexgrid/PointyLayout.hpp"

int main() {
    using namespace sub0hexgrid;
    constexpr std::size_t batch_size = 4096;
    std::array<Axial, batch_size> cells{};
    std::array<Point, batch_size> positions{};
    const auto layout = PointyLayout::TryCreate(2.0, {10.0, 20.0});
    if (!layout) return 1;
    for (std::size_t index = 0; index < batch_size; ++index) {
        cells[index] = {static_cast<int>(index % 64) - 32, static_cast<int>(index / 64) - 32};
        const auto center = layout->TryCellCenter(cells[index]);
        if (!center) return 2;
        positions[index] = {center->x + 0.17, center->y - 0.23};
        if (layout->TryCellAt(positions[index]) != cells[index]) return 3;
    }
    std::cout << "Scalar kernel, 4096 mixed-sign inputs; setup excluded.\n";
    ankerl::nanobench::Bench bench;
    bench.title("Sub0HexGrid scalar kernel").unit("cell").batch(batch_size).epochs(11);
    bench.run("checked neighbor", [&] {
        for (const auto cell : cells)
            ankerl::nanobench::doNotOptimizeAway(TryNeighbor(cell, Direction::northeast));
    });
    bench.run("wide distance", [&] {
        for (const auto cell : cells)
            ankerl::nanobench::doNotOptimizeAway(ComputeDistance(cell, {17, -23}));
    });
    bench.run("checked cell center", [&] {
        for (const auto cell : cells)
            ankerl::nanobench::doNotOptimizeAway(layout->TryCellCenter(cell));
    });
    bench.run("checked world mapping", [&] {
        for (const auto position : positions)
            ankerl::nanobench::doNotOptimizeAway(layout->TryCellAt(position));
    });
}
