# Hierarchy: initial research and integration review

Reviewed 2026-10-02. Initial research complete; architecture selection and prototype
evidence remain gates in [the phase](../phases/hierarchy-research.md). Findings below
separate source evidence from proposed project application. No source establishes
the best structure for this game's workload.

## Prior art and limits

| Direction | Primary evidence | Applicability / limitation |
|---|---|---|
| H3 aperture-7 hierarchy | [Indexing](https://h3geo.org/docs/highlights/indexing/) distinguishes exact logical from approximate geometric containment; [cell encoding](https://h3geo.org/docs/library/index/cell/) stores hierarchical resolution/child information | Useful compact-address/aggregation precedent. Spherical geometry and pentagons do not transfer directly to planar Axial. A coarse hex boundary is unsafe for pruning all logical children. |
| DGGS apertures 3/4/7 and central-place addressing | [DGGRID 8.41 manual](https://dggrid.readthedocs.io/latest/dggrid_man_V841.html) describes aperture as successive cell-area ratio and provides hierarchical address schemes | Compare parent assignment/rotation/locality; aperture alone does not prove exact regular-hex subdivision or our q/r completeness. No dependency adoption is implied. |
| Quadtree / locality encoding | [S2 hierarchy](https://s2geometry.io/devguide/s2cell_hierarchy) uses recursive four-child quadrilaterals and Hilbert-ordered IDs | Exact partition/address precedent. Spherical S2 is not a replacement for our planar fine hex cells; evaluate an axial-block version independently. |
| Linear BVH | [PBRT](https://www.pbr-book.org/4ed/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies) partitions primitives, stores enclosing bounds and flattens nodes into arrays | A query hierarchy need not have hex-shaped parents. Sparse occupied chunks/entities can use conservative bounding volumes, with build/refit costs measured. Ray-specific traversal metrics do not predict disk-query costs. |
| Parallel hierarchy construction | [Karras 2012 paper](https://research.nvidia.com/publication/2012-06_maximizing-parallelism-construction-bvhs-octrees-and-k-d-trees) and [NVIDIA explanation](https://developer.nvidia.com/blog/thinking-parallel-part-iii-tree-construction-gpu/) use Morton/radix-tree construction and bottom-up bounds | Prior art for contiguous node/key buffers and external accelerator builders. Historical device timings are not evidence for current hardware or Vulkan equivalence. |
| Broad/narrow separation | [CGAL AABB tree manual](https://doc.cgal.org/latest/AABB_tree/) separates bounds traversal from primitive intersection/distance | Useful correctness pattern; the described tree is static and not itself an all-pairs collision solution. |

Distinguish four operations before selecting representation: exact enumeration of
entities in a disk, aggregate/count queries, coarse processing/interest scheduling,
and visual LOD. The first retains inclusive fine-point semantics. Aggregates may
accept a fully covered node only for an explicitly supported reduction. LOD may
produce fewer outputs under an application approximation contract. Returning K exact
entities still requires O(K) output work; hierarchy cannot remove that lower bound.

## Project-specific hypothesis, not accepted implementation

Preserve authoritative fine Axial assignment. Compare a dense occupancy pyramid,
a sparse linear tree of occupied axial blocks, and a BVH of occupied chunks against
flat bins. Integer q/r block grouping gives exact fine-cell parentage; recursively
grouping blocks can provide multilevel processing without changing hex adjacency.
Their geometric footprints are unions of fine hexes, not larger regular hexagons.
Use mathematical floor division for negative coordinates and checked wide arithmetic.
Morton/Hilbert keys require a declared finite domain/rebase/bit budget; H2 row-major
indices must not silently change to curve order.

Derive conservative bounds from descendant cell polygons or immutable resident
points, then union them upward. Define inclusive disk/bounds tests and outward numeric
envelopes before rejecting a subtree. Independently scaled hex layouts may serve
visualization or alternate aggregation, but need their own overlap/parent assignment
proof before exact gameplay pruning. Hierarchy is an optional acceleration layer;
fine assignment and exact filtering remain the reference.

## Current receiving contracts

Inspected Crucible HEAD 7692049a2c3b596f35ab27d159efdc215b52f84e. Its
[Grid.hpp](https://github.com/CraigHutchinson/Crucible/blob/7692049a2c3b596f35ab27d159efdc215b52f84e/include/crucible/spatial/Grid.hpp)
owns copied SampleId/position data, requires exclusive rebuild/query access and
returns sorted IDs in scratch that expires on the next query/rebuild. Its
[simulation](https://github.com/CraigHutchinson/Crucible/blob/7692049a2c3b596f35ab27d159efdc215b52f84e/src/simulation.cpp)
gathers tick-start state, computes steering, applies movement and rebuilds spatial
state. [Steering](https://github.com/CraigHutchinson/Crucible/blob/7692049a2c3b596f35ab27d159efdc215b52f84e/src/swarm/Steering.cpp)
consumes IDs in order and performs floating reductions. Hierarchy traversal order
cannot silently replace deterministic consumption/replay ordering.

Crucible pins Sub0ECS 8391f81fd74a016564b4711b074eb286d3c5e14b; sibling HEAD
60285914ee8925f0ce20ac5426511604fb0c6529 is not automatically its integration base.
Reviewer checked relevant contracts at the pin: structural removal swaps rows,
migration changes placement, and partition growth can invalidate column addresses.
[World at pin](https://github.com/CraigHutchinson/Sub0ECS/blob/8391f81fd74a016564b4711b074eb286d3c5e14b/include/sub0ecs/store/world.hpp)
provides synchronous eachParallel and bounded migrateStep for storage relayout;
migrateStep is not spatial movement/dirty propagation. No public persistent bounded
gather cursor, spatial dirty stream or snapshot lease was found in that inspected API.
This is an integration gap to assess, not authorization to add generic ECS features.

Spatial snapshots must retain copied stable application IDs or snapshot row indices,
not long-lived ECS row offsets/pointers. Build bins and summaries in pending storage;
the pinned ECS handle's 24-bit slot and wrapping 8-bit generation are distinct from
SampleId and snapshot epoch. Future delayed mutation requires liveness/reuse policy.
Yielded gathers must freeze the source tick or read an owned immutable snapshot.
publish both under one epoch after completion. Tick-start steering cannot observe
post-movement summaries. Versions detect replacement but do not extend object
lifetime: yielded queries must either cancel on publication or hold a defined lease.
Incremental updates require old/new membership, predicate changes and ancestor dirty
propagation; stale bounds must never cause false-negative pruning. Slower processing
or interest ticks need an explicit staleness/swept-bound contract.

## Review outcome and unknowns

Initial integration review accepts H as a distinct research axis and keeps geometry,
occupancy, ECS storage and accelerator lifetimes separately owned. Selection is open.
Resolve cell sparsity, movement fraction, radius distribution, update/LOD rates,
hierarchy depth, world clipping/wrapping, snapshot memory ceiling, bounded gather,
deterministic ordering and permitted approximation before freezing a public API.
Both true hierarchical hex addressing and non-hex-shaped acceleration parents remain
in the comparison; the initial block hypothesis is not a foregone conclusion.
