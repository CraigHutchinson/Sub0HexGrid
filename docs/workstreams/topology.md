# T — Integer topology

Own Axial.hpp/Axial.cpp, tests/topology/ and this brief; scalar benchmark registration
remains I-owned. See the [responsibility map](responsibilities.md).
Provide checked six-direction steps and exact wide distance over all int32 q/r.
Inputs/outputs are owned values; no region, geometry, identity or storage knowledge.
Current caller is example/package; G/R and proposed H consume fine conventions.
H owns parent/group addressing; application navigation owns portals, costs and
clearance. Hierarchy does not redefine fine directions, distance or game adjacency.
Additional operations require a receiving caller.

Freeze q/r/s arithmetic and direction semantics with G/R before public changes.
Verify full-domain overflow, independent graph distances and header/package callers
using doctest. Nanobench measures scalar topology only. Handoff exact commits,
consumer audit, fixtures/results and any I-owned test/build patch requests.
