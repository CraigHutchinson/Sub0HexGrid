# H — Hierarchical addressing and conservative coverage

Status: research and architecture activity, requested 2026-10-02. No hierarchy API,
owning tree or implementation target exists. [Phase programme](../phases/hierarchy-research.md),
[source review](../research/hierarchy.md) and [use-case/spike matrix](../research/hierarchy-experiments.md)
define prioritized competing experiments. Protocol review precedes private spikes;
production architecture selection follows multiple measured rounds.

This axis covers multilevel geometric addressing, exact logical grouping, conservative
descendant coverage and the distinction between exact queries and zoom/LOD summaries.
It builds on T/G/R conventions and preserves Q's reference semantics; no circular
Q/H source dependency is proposed. It does not own ECS entities, application occupancy,
rendering policy, task scheduling, GPU memory or native compute dispatch.
H owns parent geometry and composed coverage, not production occupied traversal or
frontier storage. Those belong to application Spatial. See the
[responsibility map](responsibilities.md) for H/G/R/Q/X and consumer handoffs.

Exclusive current paths: this brief, docs/research/hierarchy.md and
docs/research/hierarchy-experiments.md. The integrator
owns the phase plan and central architecture. Future include/sub0hexgrid/hierarchy/
and tests/hierarchy/ paths are proposed only after HH-09 accepts a consumed production ADR.
No placeholders, dependencies or universal hierarchical cell ID are added now.

H hands off parent assignment, coverage proof, negative-coordinate/overflow domains,
neighbor/seam semantics, addressing widths and algorithm sources. Crucible spatial
owns occupied-node storage, summaries, update policy and snapshot publication; its
integrator owns deterministic tick/LOD consumption. Sub0ECS review verifies pinned
gather/borrow/executor contracts and proposes upstream changes only for demonstrated
gaps. X designs an accelerator descriptor only after the logical contract stabilizes.

All expensive build/update/traversal stages must allow work-bounded progress with
caller-owned state/frontiers and explicit capacity failure. Node budgets alone are
insufficient: leaf entities, sorting/aggregation and ECS gathering also need bounds.
Snapshots must publish leaves and summaries together, with cancellation or lifetime
leases defined for yielded queries. Geometry bounds must enclose actual descendants;
a display hex or aggregate approximation cannot silently become an exact pruning test.

## Experiment ownership and handoff

I owns planned experiments/hierarchy/common/ fixture/protocol adapters and shared
registration. Competing workers would own experiments/hierarchy/hs-01/, hs-02/,
hs-03/ or hs-04/ plus their local findings. Presentation owns hv-01/; navigation
owns hn-01/ and hn-02/. These are proposed sibling experiment paths, not existing targets.
Freeze immutable input, output/error/order, snapshot lifetime and counters before
assigning workers; no public backend abstraction is created to dispatch spikes.
Round handoffs include configuration, exact source/input hashes, independent oracle,
raw timing/quality/byte artifacts, tuning history and limitations per game use.
Timed runs are serialized even when spike implementation paths are disjoint.
