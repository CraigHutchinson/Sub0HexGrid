# Sub0HexGrid repository guidance

Build product-neutral C++23 value/geometry APIs with concrete consumers. Read
docs/requirements.md, docs/architecture.md and the numerical decision before code.
Coordinate operations must check overflow; geometry rejects nonfinite/unrepresentable
results. No allocation, caches, globals or exceptions in the scalar kernel.

Keep ECS, IDs, terrain, steering, Blight, pathfinding, camera, resource policy and
application storage outside this library. No speculative operation, orientation
selector, generic owning grid, polymorphic backend or dependency is added without
a receiving caller and a bounded requirement.

## Parallel workstreams

Use the reusable parallel-workstreams skill; its maintained project copy is
skills/parallel-workstreams/SKILL.md. Follow docs/workstreams/README.md and the
component brief. Architecture defines dependencies; the phase plan defines current
assignments. Planning does not dispatch agents or implement H2/H3 automatically.
Reassess retain/consolidate/split/defer at phase start and record reasons.

The integrator owns shared contracts, root CMake/exports, cmake/dependency pins,
presets, CI, central docs and current shared fixtures. Independent workers edit
only claimed component paths and send shared-file patch requests. Freeze contracts
and split shared fixtures before dependent parallel implementation; no guessed
interfaces, source globs, dummy objects or stub APIs. Use separate worktrees/build
trees; coordinate resource-heavy runs across sibling projects. Handoffs identify
SHAs, consumers, numerical/capacity/lifetime contracts, evidence and open gates.
Close each phase with reviewed integration, exact-head CI, delivery baseline and
follow-up owner/gates. Component boundaries are stable; agent assignments are not.

Correctness tests use doctest and benchmarks use nanobench, fetched through CPM
at full commits in cmake/DependencyPins.cmake. Keep both private to development
targets and out of installed runtime dependencies. Read docs/benchmarking.md before
measuring; scalar or CI smoke results do not prove large-grid/game performance.

Reference exact algorithms before implementation and derive the mapping to our
axes, radius and rounding conventions. Floating edge choices and finite-region
behavior are contracts, not incidental implementation details. Check package
consumers as well as unit tests; all exported headers must be self-contained.

Inspect worktree/status before edits. Claim paths/CPU in docs/ACTIVE_WORK_LOG.md
for sustained work. Use isolated branches and exact-path staging; preserve others'
work and artifacts. Complete coherent reviewed increments, push a PR, verify
exact-head CI and merge to main. No timing conclusions from correctness tests.

## C++ Skills

Three skills manage authoring and review. Use installed cpp-write before new public
C++ APIs, cpp-review in plan mode before implementation and after substantial code,
and cpp-simplify only from a review Rewrite Brief. Reference material is available
in the installed cpp skill directory (this workspace uses ~/.agents/skills/cpp/references).
The .cursor/rules/cpp-standards.mdc rule carries the project checklist.

Use standard-library value types in this generic project. Apply nodiscard to failure
signals and pure observations, noexcept to scalar kernel operations, pragma once
and matching primary-type filenames. Public contracts belong in headers; algorithm
explanations and source mapping belong in docs. Header order: corresponding header,
standard library, then other project headers. Load all four cpp references before
authoring/review; do not add project-specific suppression based on preference.
