# Architecture

The library has two facets: integer topology and floating world-space geometry.

- include/sub0hexgrid/Axial.hpp: Axial, Direction and checked neighbor/distance functions.
- include/sub0hexgrid/Point.hpp: the two-coordinate world value.
- include/sub0hexgrid/PointyLayout.hpp: validated radius/origin and conversion methods.
- src/: scalar implementations; no mutable/global state or heap-backed storage.
- examples/: executable consumer mapping cells and checking the topology.
- tests/: independent rule/property fixtures and self-contained header translation units.
- tests/consumer/: separate find_package caller, built from a relocated install.
- cmake/: package configuration and consumer validation script.
- docs/: requirements, numerical decisions, work breakdown and evidence.

Dependencies are acyclic: Axial/Point values -> PointyLayout; consumers -> library.
There is one concrete layout; its factory encodes positive finite geometry. Optional
results express invalid direction/overflow/conversion failure without exceptions.
No lifetime-bearing borrow leaves the kernel. Immutable layout/value operations can
run concurrently on distinct inputs; callers synchronize mutation of their own values.

Axial is a public value because q/r have no additional validity invariant. A class
with a private factory would add no safety there. PointyLayout has private validated
storage because radius/origin invariants affect every conversion. Standard optional,
array and scalar values suffice; there is no backend/interface registry.

The package exports Sub0HexGrid::Sub0HexGrid as a compiled static library with C++23
and install-relative include paths. Tests/examples/options are gated for embedding.
A package consumer links only the exported target. Sanitizers are opt-in and do not
alter installed interface requirements; sanitizer builds test their own instrumented
consumer without becoming a production package configuration.

Crucible may later use the geometry through its spatial module. Its SampleId,
sorted bins, exact radius results and world limits are not imported here. Before
adoption, a new region/candidate facet must specify enumeration and capacity. Any
migration of Blight changes game adjacency separately. Pointy-top choice is the
first concrete library layout, not a forced decision for Crucible's final world.
