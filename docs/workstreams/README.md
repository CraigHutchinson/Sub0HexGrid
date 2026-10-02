# Workstream catalog

Architecture is the contract authority. This catalog follows Crucible's separation
of durable components from phase-specific agent assignments. Status is capability
status, not an active file claim. Read AGENTS.md, architecture.md, work-breakdown.md,
the stream brief and ACTIVE_WORK_LOG.md before starting.

| ID | Brief | Exclusive implementation paths | State / receiving caller |
|---|---|---|---|
| I | [Integration](integration.md) | Root CMake, cmake/, presets, CI, AGENTS, central docs, examples/, tests/consumer and headers; benchmarks/kernel.cpp and spatial.cpp | Package/examples, H2 combined evidence |
| T | [Topology](topology.md) | include/sub0hexgrid/Axial.hpp, src/Axial.cpp; future tests/topology/, benchmarks/topology/ | Implemented scalar; example/package |
| G | [Geometry](geometry.md) | include/sub0hexgrid/Point.hpp and PointyLayout.hpp, src/PointyLayout.cpp; future tests/geometry/, benchmarks/geometry/ | Implemented scalar; example/package |
| R | [Regions](regions.md) | include/sub0hexgrid/regions/, src/regions/, tests/regions/; future benchmarks/regions/ | Implemented; standalone spatial/package consumer |
| Q | [Candidates](candidates.md) | include/sub0hexgrid/candidates/, src/candidates/, tests/candidates/; future benchmarks/candidates/ | Implemented; standalone spatial/package consumer |
| X | [Interoperability](interoperability.md) | Future include/sub0hexgrid/interop/, tests/interop/; implementation only when justified | Proposed; named external backend required |

Each stream owns its named brief. New folders/manifests are planned boundaries,
not existing code or reserved targets. Existing scalar headers are not moved just
to fit a folder pattern. Shared tests must be split preserving fixtures before T/G
can edit independently; until then they submit exact patches to I.

Phase plans and close records live in [phases/README.md](../phases/README.md).
The [next H2 sprint](../phases/h2-bounded-spatial.md) selects I + R/Q with explicit
contract/fixture handoffs; T/G maintenance is consolidated with I and X is deferred.
Execution was explicitly authorized; [H2 delivery](../phases/h2-delivery.md)
records integration, reviews and the next refinement. T/G fixtures are now split.

## Dispatch and shared surfaces

At each phase start record retain/consolidate/split/defer decisions, outcome, owner,
branch/worktree/base SHA, exclusive paths, prerequisite contract, real caller, gates
and bounded stretch. Planning is not dispatch. Prefer the fewest workers that can
deliver independent consumed increments; do not spawn workers to fill slots.

I owns root inventories, exports, dependency pins/CPM, presets, CI and central docs.
Workers submit shared-file patch requests describing the change and affected callers.
I applies them serially. Local source/test/benchmark manifests use explicit source
lists and private development dependencies. Once wired by I, workers can edit their
local manifests without touching the root. No globs, dummy objects or stub-success APIs.
Freeze the test framework and local registration convention before that split.

Cross-stream contract changes name the owner and all affected consumers, include
the numerical/ownership/capacity consequences, and are reviewed before dependents
apply them. Do not independently redefine coordinate axes, rounding, index width,
boundary rules or ordering in another module.

## Parallel-ready gates

- T and G can work independently after shared fixtures/manifests are split and their
  unchanged dependency contract is agreed.
- R may develop against frozen T. Q may research and write independent oracles while
  R is developed, but its implementation waits for the region/index contract.
- X may specify parity fixtures after G/R/Q contracts freeze; adapter work waits for
  a named backend caller and capability audit.
- Package validation can proceed on an immutable handoff commit. CPU-heavy tests,
  measurements and shared build directories are never implicitly parallel-safe.

Use separate worktrees/build trees where possible, inspect status before and after
work, claim paths/resources in ACTIVE_WORK_LOG.md, and stage exact paths. CPM sources
are read-only. Preserve unrelated dirt, live worktrees and useful artifacts. Coding
in parallel does not authorize parallel mutation of application bins or GPU buffers.

## Handoff and integration

Every handoff records task ID, exact commits/base, paths, public contracts, real caller,
dependency/pin changes, actual commands/results, remaining limitations and shared patch
requests. Include read/write sets, borrow expiry and completion barriers for runtime
parallelism; GPU completion must cover all uses before buffers can be reused/released.
Cpp-review and independent numerical fixtures are separate gates. Tests use doctest;
benchmarks use nanobench, both through CPM. Filtered tests aid iteration; integration
requires the unfiltered suite, package consumer and relevant header checks.

I integrates prerequisite commits first, verifies compatibility and combined evidence,
resolves findings, then publishes a reviewable PR and verifies exact-head CI before
merge. Close the phase with delivered scope, findings, stable follow-up IDs/owners,
evidence, merge baseline and the next division of work. Update claims and retain
artifact-bearing worktrees unless cleanup is explicitly safe and authorized.
