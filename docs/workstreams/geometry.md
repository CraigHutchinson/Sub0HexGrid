# G — World geometry

Own point/pointy_layout headers, pointy_layout.cpp, tests/geometry/ and this brief.
Depend on T; retain positive finite radius/origin, checked
conversion and documented computed-double rounding. Current caller is example/package.

Bulk mapping requires a real caller, caller-owned output, invalid-element semantics,
overlap/lifetime rules and comparison to scalar behavior. Preserve precision-collapse
rejection in tryCellCenter. Coordinate scale, ties and changed descriptor semantics
require affected Q/H/X agreement through I; no silent precision switch for GPU convenience.
G owns primitive transforms/numeric assignment; H composes descendant coverage.
Camera projection, mini-map/zoom thresholds and visual error belong to Presentation.
See the [responsibility map](responsibilities.md).

Doctest fixtures cover analytic seams, independent nearest-center search, extreme
coordinates and multiple scales/origins. Nanobench separates mapping and checked
center cost. Handoff exact commits, numerical decisions, results and shared patches.
