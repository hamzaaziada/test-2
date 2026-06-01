# ADR 0001 — Layout-solver technique for Phase-0 label placement

- **Status:** Accepted (Phase 0)
- **Context tasks:** T-0002 (solver-technique spike), T-0016 (solver v1)
- **Deciders:** Principal Architect, Layout-Solver Engineer, QA Lead
- **Requirement:** FR-B01 (collision-free label placement), FR-I07 (deterministic equivalence)

## Problem

Discrete label placement (interior-segment vs. external-with-leader-line, collision-free) is NP-hard in general (map-labeling). We need a Phase-0 technique that is (a) **deterministic** — same input → identical output, for the equivalence harness — and (b) **incremental-friendly** for fast re-solve on data change.

## Alternatives considered

1. **Greedy sequential placement with ranked candidate slots + external-overflow fallback.** Each label tries interior candidates in priority order, accepting the first that is in-canvas and collision-free against placed boxes + obstacles; overflow to a well-separated external slot with a leader line. O(L·C·(O+L)).
2. **ILP / exact optimization.** Globally optimal, but heavy, non-incremental, and risks non-determinism across solver versions; overkill for Phase 0.
3. **Simulated annealing / local search.** Good quality on hard instances but stochastic — fails the determinism requirement unless seeded and pinned, and harder to reason about.

## Decision

Adopt **Alternative 1 (greedy + ranked candidates + external overflow)** for Phase 0. It is trivially deterministic (pure `std::vector` iteration, no hash containers), cheap, and clears the collision gate on the fixture set. ILP/SA are retained as **fallbacks for hard cases** in later phases (TRD §9, TQ-2) behind the same `placeLabels` contract — so the technique can be swapped without changing callers.

## Consequences

- (+) Deterministic and fast; passes FR-I03 (overlap=0) on the ~Phase-0 fixtures.
- (+) Stable contract (`solver.hpp`) lets us upgrade the technique later without API churn.
- (−) Greedy is not globally optimal; dense adversarial cases (PRD §5.2) may need local-repair or ILP escalation — revisit in Phase 1 (T-1020) with oracle/perf data.
