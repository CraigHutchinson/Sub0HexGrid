# Architecture

## Component boundaries and parallel development

User direction, 2026-10-02: support large grids, efficient memory and runtime APIs,
and external accelerator implementations. The implemented scalar kernel remains
the numerical reference. The table distinguishes current code from proposed work;
it does not authorize implementation or create public APIs/targets.

| Component / stream | Responsibility | Inputs and outputs | Dependency / status |
|---|---|---|---|
| Topology T | Integer lattice values, checked steps and exact distance | Axial + Direction -> optional Axial; Axial pair -> uint64 distance | No geometry/storage dependency; implemented |
| Geometry G | Validated pointy layout and checked world mapping | Immutable layout + Point/Axial -> optional Axial/Point | T + Point; implemented |
| Regions R | Finite membership and compact coordinate/index mapping | AxialRegion inclusive rectangle + Axial/uint64 index -> checked membership/index/coordinate | T; implemented H2 |
| Candidates Q | Complete conservative world-radius candidate traversal | G layout + R region + query disk -> clipped conservative rectangle, slices and cursor batches | G + R; implemented H2 |
| Hierarchy H | Multilevel addressing, logical grouping and conservative descendant coverage | Frozen fine cells and level/group rules -> proposed parent/bounds contracts | T/G/R conventions; compatible with Q reference, research phase, no API |
| Interoperability X | External-kernel descriptor, buffer layout and parity specification | Frozen G/R/Q conventions -> explicitly laid-out descriptor and fixtures | G + R + Q; proposed, consumer-gated |
| Integration I | Contracts, package/build inventory, dependencies, CI, cross-component evidence | Stream handoffs -> reviewed combined package and consumer evidence | Owns shared wiring; current |

The workstream catalog is [workstreams/README.md](workstreams/README.md). It defines
exclusive paths, prerequisites, handoff contents and parallel-ready gates. Component
boundaries are durable; phase assignments may retain, consolidate, split or defer
them. H2 implementation and evidence are recorded in [its close record](phases/h2-delivery.md). Existing flat public include paths remain
stable; new component folders are introduced only alongside consumed implementation.

Dependency direction is T -> G, T -> R, G/R -> Q, G/R/Q -> X; I composes and validates.
T never calls G; R never calls world geometry; Q neither stores entities nor performs
gameplay filtering. X does not introduce CUDA/Vulkan dependencies into the kernel.
Changes to shared conventions need the integrator and affected stream owners before
dependent implementation proceeds. Public contract decisions belong in headers and
docs/decisions; ownership/process decisions belong in the catalog and phase plan.

## Storage and algorithm contracts for H2

The accepted [bounded-work decision](decisions/0002-bounded-regions-and-work.md)
defines the implemented surface. AxialRegion stores constant-size metadata with
checked uint64 count/index arithmetic. CandidateCells owns conservative rectangle
metadata; construction and checked contiguous slices are O(1), traversal is O(emitted
cells). CandidateCursor writes a caller-owned span and retains progress. Queries
reject invalid/unrepresentable arithmetic, including normalized intervals outside
the documented binary64 magnitude envelope. False positives remain explicit.

The standalone SpatialIndex example owns preallocated current/pending dense bins.
Count/scan/scatter rebuild phases and queries resume under explicit work budgets.
Query work counts both cell visits and entity examinations, including mid-bin yields.
Applications check deadlines between batches; a batch is a work bound, not a hard
time guarantee. Input borrows expire on commit/fail/cancel; queries require their
index to outlive them and are invalidated by publication. No concurrent mutation is
allowed in this example. This is an application pattern, not exported grid storage.

Lattice coordinate, region membership, compact cell index and application entity ID
are distinct concepts. Region metadata describes storage addressing without owning
terrain or entity bins. Start with one finite shape consumed by Crucible; validate
all extents, index arithmetic and byte counts. Dense arrays are the baseline for
resident cells. A sparse directory of dense chunks is a later application storage
choice; negative-coordinate division, absent chunks and cross-chunk queries need
explicit contracts before adoption. Do not allocate one object or neighbor table
per regular cell without workload evidence.

Candidate traversal must cover every cell capable of containing an entity within
the inclusive world-space query disk, including cells whose centers are outside it.
A lattice step disk is a different operation. Derive conservative bounds from the
actual pointy convention and verify against brute-force point membership. Prefer
clipped row intervals with deterministic order and independent partitioning; no
allocation, retained inputs, silent truncation or full-region scan for local queries.
Freeze iteration, clipping, numeric, cancellation/capacity and failure semantics
before selecting the public range/buffer API. Count arithmetic must also be checked.

Applications own bin construction, entity IDs, exact distance filtering and query
outputs. Immutable committed bins plus disjoint caller-owned output make independent
queries possible. Count/scan/scatter and sorted cell-key ranges are candidates for
application CPU/GPU builders; neither is mandated by this geometry library. Global
ID sorting and repeated identity lookup are consumer requirements to justify,
not compulsory traversal behavior. Complete dense neighborhoods can still require
quadratic work; occupancy limits or approximate steering belong to the application.

## Hierarchical processing and zoom

Hierarchy is a separate [workstream](workstreams/hierarchy.md), with an initial
[source/integration review](research/hierarchy.md) and a [research-to-architecture
phase](phases/hierarchy-research.md). Logical parentage, geometric containment,
exact query acceleration and visual LOD are distinct contracts. Compare true
multiresolution hex schemes with exact axial block grouping, occupancy pyramids,
sparse linear trees and BVHs; no shape/backend has been selected. The
[use-case programme](research/hierarchy-experiments.md) prioritizes motion, mini-map
and top-down pan/zoom, with navigation reference/contracts defined alongside them.
Shared-goal fields, portal graphs and view caches are independent comparison axes.
Multiple private spikes and measured refinement rounds precede the production ADR;
a protocol ADR authorizes experiments without predetermining their outcome. Separate
motion, view and navigation representations remain valid eventual choices.

H may supply consumed geometric grouping/bounds rules. Crucible owns occupied
hierarchy arrays, summaries, updates, snapshot publication and zoom/processing policy.
Sub0ECS retains entity/component ownership; integration verifies bounded gathering,
stable identity and borrow barriers against the actual pinned version. Publish leaves
and summaries together. Node and leaf-entity progress must both remain resumable;
coarse display geometry cannot silently prune logical descendants. H hands stable
contracts to X if an external accelerator caller is selected.

## External compute contract

X defines an interchange contract only with a named external CUDA/Vulkan caller.
Specify fixed-width fields, bounds, origin/scale, index convention, offsets, strides,
validity and alignment; assert C++ layout and verify shader buffer layout. C++
optional/private object bytes and size_t are not portable shader representations.
No backend registry, virtual dispatch, allocator, command queue or device handles
enter the scalar API. External adapters own compilation, uploads, device-resident
buffers, dispatch, synchronization and readback lifetime.

Select precision and parity explicitly: exact integer topology/indexing; documented
geometry tolerances and seam policy; CPU-authoritative assignment or conservative
cross-backend candidates when bins differ. Existing computed-double ties do not
promise CPU/GPU identity. Rebased local coordinates may address large-world precision
but require a consumer decision, not a silent change to Axial. Bulk CPU mapping is
considered by G with a real caller, reusable output and per-element failure semantics.

Performance gates start during H2 design, before adoption. Measure storage bytes,
scratch, rebuild/query cost, occupancy and candidate amplification. External compute
claims include transfers, dispatch, synchronization and rendering contention; scalar
nanobench results establish only scalar costs. See [benchmarking](benchmarking.md).

## Current scalar package

The library has four facets: topology, geometry, finite regions and candidates.

- include/sub0hexgrid/Axial.hpp: Axial, Direction and checked neighbor/distance functions.
- include/sub0hexgrid/Point.hpp: the two-coordinate world value.
- include/sub0hexgrid/PointyLayout.hpp: validated radius/origin and conversion methods.
- include/sub0hexgrid/regions/AxialRegion.hpp: checked compact finite indexing.
- include/sub0hexgrid/candidates/: owned candidate range, slices and resumable cursor.
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
