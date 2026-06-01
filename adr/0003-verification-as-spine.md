# ADR 0003 — Verification is the spine: the oracle is a build gate, not a report

- **Status:** Accepted (Phase 0)
- **Context tasks:** T-B012 (oracle skeleton), T-0030 (oracle wired into CI as hard gate)
- **Deciders:** QA / Verification Lead, Orchestrator
- **Requirement:** FR-I01 (layout-quality oracle), FR-I03 (collision hard gate)

## Problem

R-4: visual layout quality has no automatic "is this right?" signal by default, so verification — not coding — is the real bottleneck. If the oracle is a passive report, regressions slip through.

## Decision

The layout-quality oracle (`verification/oracle`) is wired as a **CTest test that fails the build** when `overlappingPairs != 0` or `offCanvasLabels != 0` on any fixture (`tests/test_e2e_gate.cpp`). Overlap = 0 is a **hard gate**, exactly as FR-I03 mandates. CI (`.ci/`) runs `ctest` on every change; a geometry regression turns the build red and blocks merge (Master Part E).

## Consequences

- (+) "Done" is evidence-bound: the collision gate is mechanical, not a screenshot (Master Directive 1).
- (+) Every new chart type / edge case adds fixtures to the same gate (FR-I02 corpus grows; each fixed bug → a fixture).
- (−) The oracle covers geometry/overlap/alignment only — it cannot certify "looks like a human made it." That residual perceptual judgment stays a **human checkpoint at Gate 3** (Master Part G), not an automated claim.
