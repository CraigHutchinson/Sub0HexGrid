#pragma once

namespace sub0hexgrid {
/// World-space coordinates; geometry conversion explicitly rejects nonfinite values.
struct Point {
    double x{};
    double y{};
};
}
