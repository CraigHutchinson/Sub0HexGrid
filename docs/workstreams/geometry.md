# G — World geometry

Own Point/PointyLayout headers, PointyLayout.cpp and this brief; shared tests remain
I-owned until split. Depend on T; retain positive finite radius/origin, checked
conversion and documented computed-double rounding. Current caller is example/package.

Bulk mapping requires a real caller, caller-owned output, invalid-element semantics,
overlap/lifetime rules and comparison to scalar behavior. Preserve precision-collapse
rejection in TryCellCenter. Coordinate scale, ties and changed descriptor semantics
require Q/X agreement; no silent precision switch for GPU convenience.

Doctest fixtures cover analytic seams, independent nearest-center search, extreme
coordinates and multiple scales/origins. Nanobench separates mapping and checked
center cost. Handoff exact commits, numerical decisions, results and shared patches.
