# H — Hierarchical addressing and conservative coverage

Status: research and architecture activity, requested 2026-10-02. No hierarchy API,
owning tree or implementation target exists. [Phase plan](../phases/hierarchy-research.md)
and [initial source review](../research/hierarchy.md) define the current deliverable.

This axis covers multilevel geometric addressing, exact logical grouping, conservative
descendant coverage and the distinction between exact queries and zoom/LOD summaries.
It builds on T/G/R conventions and preserves Q's reference semantics; no circular
Q/H source dependency is proposed. It does not own ECS entities, application occupancy,
rendering policy, task scheduling, GPU memory or native compute dispatch.

Exclusive current paths: this brief and docs/research/hierarchy.md. The integrator
owns the phase plan and central architecture. Future include/sub0hexgrid/hierarchy/
and tests/hierarchy/ paths are proposed only if an accepted ADR names a real caller.
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
