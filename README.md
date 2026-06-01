# Lattice — Full-Parity think-cell Competitor (Spec Repository)

> A PowerPoint + Excel add-in targeting **full functional parity with think-cell**: automatic collision-free label placement, the full chart + Gantt + agenda + annotation surface, live Excel data-linkage, brand compliance, and graceful degradation to native PowerPoint objects for non-users.

This repository is the **Spec-Driven Development (Spec Kit)** source of truth. Code is the expression of these documents — when code and spec disagree, the spec wins (or an ADR records the deviation).

## Read in this order

1. **[`PRD-lattice.md`](./PRD-lattice.md)** — *what & why* (v2.0). Requirements `FR-*`/`NFR-*`, personas, scenarios A–F, the parity definition (§1.4), Definition of Done (§12).
2. **[`TRD-lattice.md`](./TRD-lattice.md)** — *how* (v1.0, `plan.md`). Portable headless core + thin host-adapters; the layout solver moat; verification machinery; risks R-1…R-7.
3. **[`MASTER-BUILD-PROMPT.md`](./MASTER-BUILD-PROMPT.md)** — the multi-agent engineering company: roster, decision protocols, **stage gates**, continuous checks, human checkpoints.
4. **[`tasks.md`](./tasks.md)** — the dependency-ordered, `[P]`-tagged, test-first build backlog (the next Spec Kit artifact, generated from the TRD module boundaries).

## Living state

| Artifact | Purpose |
|--|--|
| [`parity-ledger.md`](./parity-ledger.md) | The acceptance instrument (FR-I06): one row per FR → test → status. **Done = 100% `parity`.** |
| [`risk-register.md`](./risk-register.md) | R-1…R-7 live status + human-checkpoint linkage. |
| [`complexity-tracking.md`](./complexity-tracking.md) | Justified constitution-gate exceptions (TRD §14). |
| [`adr/`](./adr/) | Architecture Decision Records. |
| [`contracts/`](./contracts/) | Engine API, host-adapter, connectors, style-file (+ contract tests). |
| [`implementation-details/`](./implementation-details/) | Solver spec, data model, perf budgets, traceability. |

## Repository layout

```
core/                 portable engine (TRD §4)
adapters/windows  adapters/macos  adapters/officejs
services/automation  services/scanner
verification/         oracle, equivalence harness, corpus, cross-Office grid (Epic I)
.ci/                  pipeline (Part E continuous checks)
```

## The four prime constraints (never trade away)

- **Parity is evidence-bound** — nothing is "done" on a demo; only on ledger + equivalence evidence.
- **Verification is the spine, not a phase** — the oracle/equivalence/corpus/collision gates are built *before or alongside* the features they judge.
- **Moat first** — the collision-free layout solver (FR-B01) is built and oracle-gated in Phase 0 before any breadth work.
- **Respect human checkpoints** — CL-A (pin baseline), CL-1 (platform), R-6 (IP counsel), perceptual sign-off, release approval.

## Open human decisions (block progress — see `tasks.md` Phase −1)

| ID | Decision | Provisional (TRD §1) | Blocks |
|--|--|--|--|
| **CL-A** | Pin think-cell baseline version + date | 14.x GA, 2026-06-01 | all gates / DoD |
| **CL-1** | Native add-in vs Office.js (post R-2 spike) | native + portable core | entire build |
| **CL-B** | Include think-cell 15 / Assist (Epic J)? | No | Epic J |
| **CL-9** | Commercial model | out of scope; pluggable slot | FR-H09 only |

---
*Status: spec + plan + backlog complete; awaiting Phase −1 bootstrap (contracts v0, oracle skeleton, CL-A pin).*
