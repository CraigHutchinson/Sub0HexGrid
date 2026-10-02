# Q — Complete spatial candidate traversal

Implemented H2 component: CandidateCells, slices and CandidateCursor. Own candidates
folders and this brief. Depend on frozen G and R contracts. Receiving caller is the
standalone spatial example; Crucible adoption remains H3. Application bins,
exact entity filtering, identity and result sorting remain application-owned.

Derive conservative inclusive world-disk bounds from the accepted pointy convention.
Freeze clipped row/cell traversal, deterministic order, duplicate rules, partitioning,
capacity and invalid-query behavior. Six neighbors or center-only disk filtering are
not a completeness proof. Do not scan a whole region for a local query.

Before R freezes, independent brute-force oracles/source derivation can proceed;
implementation cannot guess R's indexing. Doctest covers zero radius, seams, borders,
large radii, negatives and overflow without silent omissions. Nanobench records
candidate amplification and traversal across sparse/clustered scenarios when the
caller exists. Handoff exact commits, derivation, completeness evidence and limitations.
