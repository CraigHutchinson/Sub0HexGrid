# Sub0HexGrid

A small C++23 library for reusable hexagonal topology and geometry. Crucible is the
first planned application consumer: swarm positions can be assigned to hex cells
while entity bins, exact radius filtering and gameplay remain application-owned.

The first increment provides axial values, checked six-neighbor lookup, wide
topological distance and validated pointy-top world/cell mapping. The example and
installed-package consumer exercise these APIs. Finite regions, complete query
candidate traversal and Crucible migration are subsequent gated increments.

Read [requirements](docs/requirements.md), [architecture](docs/architecture.md),
[numerical decisions](docs/decisions/0001-coordinate-layout.md), and
[work breakdown](docs/work-breakdown.md). [Validation](docs/validation.md) records
actual checks and limits. No performance or full-world hex migration claim is made.

## Build and use

Requires CMake 3.25+, Ninja and a C++23 toolchain. There are no external dependencies.

```sh
cmake --preset debug
cmake --build --preset debug --parallel 4
ctest --preset debug
./build/debug/sub0hexgrid_example
cmake --install build/debug --prefix build/install
```

Consumers use `find_package(Sub0HexGrid CONFIG REQUIRED)` and link
`Sub0HexGrid::Sub0HexGrid`. The target supplies include paths and C++23 requirements.
See [CONTRIBUTING.md](CONTRIBUTING.md) for Release, sanitizer and consumer checks.

Pointy-top layout uses a positive circumradius, finite world-space origin and
positive y in the r-axis direction. It does not select a renderer or a viewport.
Invalid geometry/conversions return nullopt; integer neighbor overflow never wraps.
Hex edges/vertices use documented computed-double rounding, not a cross-platform
floating-point identity promise.

Crucible currently retains its verified rectangular bins. Adopting hex swarm bins
and changing cellular Blight adjacency are separate decisions. The geometry kernel
alone cannot provide complete radius candidates or prove game performance.

Project license has not yet been selected; no external source code was copied.
Algorithm references and convention mapping are recorded in the numerical decision.

## Preserved extensions

[Future surface requirements](docs/future-surfaces.md) preserve local planar terrain
height and possible hexasphere-like subdivision. Terrain resources, mining/erosion,
movement cost and permanent nanite construction remain receiving-application rules.
The initial planar kernel does not implement spherical adjacency or terrain state.
