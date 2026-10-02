---
name: parallel-workstreams
description: Define or maintain component boundaries, workstream catalogs, phase assignments and handoffs for repositories developed by independent agents. Use for parallel project architecture or multi-agent coordination methodology, not ordinary single-file edits.
---

# Parallel workstreams

Turn the requested outcome into independently reviewable increments around stable
component contracts. Preserve existing repository conventions and authorization;
a catalog or phase proposal does not itself authorize implementation or worker dispatch.

## Architecture and catalog

Read the project's AGENTS.md, architecture, current workstream/phase records and
active work log. When a parent project is named, inspect its actual methodology
and adapt the useful boundaries rather than importing unrelated product policy.
Distinguish implemented components, planned boundaries and active assignments.

For each component record responsibility, named receiving caller, inputs/outputs,
dependencies, exclusive source/test/docs paths, shared surfaces and acceptance gates.
Keep ownership, borrow expiry, capacities, errors, ordering and numerical policy
explicit at interaction boundaries. Maintain an acyclic dependency direction.
Do not create placeholder APIs, targets, source files or abstractions merely to
make the catalog appear implemented. Preserve established public paths.

Choose assignments per phase: retain, consolidate, split or defer with reasons.
Use disjoint paths and stable contracts, not the number of available agent slots.
Record outcome, owner, branch/worktree, base SHA, prerequisite contracts, gates and
bounded stretch. Unfrozen dependencies block dependent implementation; independent
research/oracles may proceed without inventing the missing contract.

## Coordination and execution

One integrator owns root inventories, dependency pins, presets, CI, exports and
central docs. Workers own component-local manifests once wired and send precise
shared-file patch requests to the integrator. Contract changes involve affected
owners/callers before dependent edits; enumerate consumers repo-wide.

Inspect worktree/status and resource claims before starting. Prefer isolated
worktrees/build trees, exact-path staging and read-only dependency source caches.
Claim sustained paths and heavy CPU/GPU runs; do not assume old claims expired.
Parallel development is distinct from runtime concurrency: runtime tasks declare
reads/writes, partition ownership, completion barriers and buffer/borrow lifetimes.
Serialize contended measurement runs even when implementation paths are disjoint.

Dispatch only within the user's or applicable project instruction's authorized
scope. Give each worker a bounded task, owned paths, exact base/contracts, receiving
caller, gates and stop condition. Keep model/worker limits project-specific.

## Handoff and close

A handoff names exact commits/base, changed paths/contracts, consumer wiring,
shared patches, actual commands/results, limitations and unresolved prerequisites.
Keep code-quality review separate from correctness and measured performance.
Use filtered checks for iteration and combined/package checks at integration.
Performance claims identify workload, hardware, residency and timing scope.

Integrate dependencies first, review the combined result and complete the project's
authorized delivery workflow. Verify exact-head CI when publishing/merging; do not
infer delivery authorization from this skill alone. Close with delivered scope,
findings/resolutions, stable follow-up IDs/owners/gates, evidence, merged baseline
when applicable and the next proposed division. Update claims and preserve unrelated
work/artifacts; cleanup only known-owned, authorized resources.
