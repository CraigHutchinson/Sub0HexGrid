# Sub0HexGrid repository guidance

Build product-neutral C++23 value/geometry APIs with concrete consumers. Read
docs/requirements.md, docs/architecture.md and the numerical decision before code.
Coordinate operations must check overflow; geometry rejects nonfinite/unrepresentable
results. No allocation, caches, globals or exceptions in the scalar kernel.

Keep ECS, IDs, terrain, steering, Blight, pathfinding, camera, resource policy and
application storage outside this library. No speculative operation, orientation
selector, generic owning grid, polymorphic backend or dependency is added without
a receiving caller and a bounded requirement.

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
