#include <array>
#include <iostream>

#include <sub0hexgrid/PointyLayout.hpp>

int main() {
    using namespace sub0hexgrid;
    const auto layout = PointyLayout::TryCreate(2.0, {10.0, 20.0});
    if (!layout) return 1;
    constexpr Axial start{2, -1};
    const auto center = layout->TryCellCenter(start);
    if (!center || layout->TryCellAt(*center) != start) return 2;
    constexpr std::array directions{Direction::east, Direction::northeast, Direction::northwest,
                                    Direction::west, Direction::southwest, Direction::southeast};
    for (const auto direction : directions) {
        const auto neighbor = TryNeighbor(start, direction);
        if (!neighbor || ComputeDistance(start, *neighbor) != 1) return 3;
    }
    std::cout << "Sub0HexGrid: q=2 r=-1 center=(" << center->x << ',' << center->y
              << ") neighbors=6 round_trip=exact\n";
}
