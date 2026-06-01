# complexity-tracking.md — Lattice

> Spec Kit constitution requires that any gate failure be **justified**, not hidden. Source: `TRD-lattice.md` §2 (Phase −1 gates) and §14. Ratified in bootstrap task T-B004.

## Constitution gate evaluation

| Gate | Result | Justification | Mitigation |
|--|--|--|--|
| **Simplicity Gate** (≤3 projects, no future-proofing) | **FAIL (exception granted)** | Full parity with a decade-plus product spans ≥6 genuinely separable subsystems: portable core, Windows adapter, macOS adapter, connectors, automation, verification. Collapsing them would couple brittle host code to the pure engine and break headless testing. | Strict module boundaries + contract tests bound effective complexity; each module is an independently testable library. |
| **Anti-Abstraction Gate** (use frameworks directly, single model) | **PARTIAL FAIL (exception granted)** | The Host-Adapter layer deliberately wraps the Office object model. The abstraction *is* the mitigation for R-1 and the enabler of Win/Mac parity + headless testing. Using the Office API directly everywhere would spread brittleness through the whole codebase. | Abstraction is thin (I/O only); the core never sees COM. One scene-graph model, not parallel models. |
| **Library-First** (Article I) | PASS | Engine is a standalone host-independent library; every subsystem a bounded module. | — |
| **CLI Interface** (Article II) | PASS | Engine exposes headless CLI/JSON — load-bearing: serves automation (FR-E08) **and** the test oracle (FR-I01/I07). | — |
| **Test-First** (Article III) | PASS (by mandate) | Contracts + contract tests precede implementation; red-first enforced in tasks.md. | CI blocks impl PRs lacking red contract tests. |
| **Integration-First** (Article IX) | PASS (by mandate) | Adapter tests run against real PowerPoint/Excel instances; mocks only below the adapter boundary. | Cross-Office grid (FR-I04). |

## Open exception reviews

- Exceptions above are **ratified for the duration of v1 parity**. Re-evaluate if CL-1 forces Mac onto Office.js (would collapse one subsystem but split fidelity — re-justify).
- No new complexity exception may be merged without an entry here + Architect sign-off (Master Part C5 review checklist).
