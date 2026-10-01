# Project requirements

Status: first scalar kernel increment. Crucible's continuous-position spatial
index is the first planned receiving caller; it keeps storage/identity and precise
point filtering. The standalone example/package consumer is the current executable
integration. This library is independent of all sub0/application dependencies.

## Functional contract

| ID | Requirement | Initial acceptance |
|---|---|---|
| R1 | Axial q/r coordinates are signed 32-bit values; implicit s is evaluated wide | Full-range integer fixtures, equality/order and trivial value contract |
| R2 | Six fixed neighbor directions and checked overflow | Each direction, inverse steps, invalid enum and extreme coordinates |
| R3 | Topological distance is exact over the full q/r domain | Wide result, symmetry/triangle and independent shortest-path fixtures |
| R4 | Pointy-top regular layout uses positive finite circumradius and finite origin | Factory rejection and translated/scaled center fixtures |
| R5 | World/cell mapping defines numeric and rounding failures | Centers/negative coordinates, nearest-center oracle, boundary ties, nonfinite/out-of-range cases |
| R6 | Scalar operations allocate no storage and retain no caller data | Value-only implementation and review; no global state or allocator dependency |
| R7 | Independently installed CMake package works without source/build paths | Relocated consumer build, self-contained header translation units |
| R8 | Region/candidate traversal preserves completeness and bounded capacity | Deferred until region contract and real consumer are frozen |

R8 is required before replacing Crucible's radius-query index; this initial kernel
does not imply a complete spatial backend. No boundary clipping/wrap is implicit
in the infinite coordinate lattice. Finite map shape, cell storage mapping and
inclusive disk candidate enumeration need a separate design/fixture increment.

## Numerical scope

All q/r bit patterns are valid coordinates, not a bounded-map membership claim.
Implicit s can exceed int32 and must not be stored by narrowing. Translation uses
wide intermediates and returns nullopt when resulting q/r cannot fit. Distance
returns uint64, so even opposite int32 endpoints remain representable.

Mapping uses double and checks every exported result before narrowing. Valid layout
does not guarantee every coordinate center or world point is representable for its
radius/origin. Reject conversion overflow rather than clamping to an unrelated cell.
Tie rules apply to computed doubles; math-library/compiler/platform differences near
seams are not guaranteed to select the same cell.

## Excluded initial scope

Application IDs/ECS, entity bins, owning grids, arbitrary scalar templates, flat-top
layout, offsets/doubled-coordinate adapters, rings/pathfinding/navigation, rendering,
thread scheduling and resource rules remain outside this increment. Add operations
with named receiving callers rather than a full hex framework.

## Completion gates

Plan/code review; independent topology/geometry fixtures; supported Debug/Release,
ASan/UBSan; installed consumer and header checks; actual evidence and CI; reviewed
merge. No benchmark/FPS claims. Upstream reuse and later Crucible adoption are
recorded separately with the exact commit and consumer evidence.

## Preserved future requirement

See [surface extensions](future-surfaces.md) for planar height and hexasphere-like
worlds. Their adjacency, projection, height and material semantics require separate
facets/consumer decisions. No initial API is a generic sphere/terrain interface.
