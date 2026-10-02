# R — Finite regions and compact indexing

Proposed H2 component; no implementation dispatched. Own future regions folders
listed in the catalog and this brief. Depend on T only. Receiving caller must be
Crucible's spatial adapter; the application owns all backing field/entity storage.

First freeze one consumed finite shape, membership, coordinate/index bijection,
iteration order, capacity/index widths and checked dimension/byte arithmetic with I/Q.
Region descriptors are immutable metadata, not a generic owning grid. Chunk directories,
terrain fields and application world identity remain outside this increment.

Doctest must establish bijection, holes/edges/empty or rejected extents, negatives,
limits and independent membership. Benchmark contiguous traversal/indexing and metadata
bytes at realistic sizes. Handoff the contract and fixtures before Q implementation;
return explicit failures, retain no buffers, and report actual consumer evidence.
