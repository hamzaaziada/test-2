# risk-register.md — Lattice

> Live register. Source: `TRD-lattice.md` §13. Owned by the Orchestrator; reviewed at every Gate and retro (Master Part C7). Status updates as spikes/gates resolve.

| ID | Risk | Sev | Owner | Status | Mitigation / current action | Linked tasks |
|--|--|--|--|--|--|--|
| **R-1** | Office object-model brittleness / COM-STA threading — bugs appear only on real Office | High | Windows-Adapter Eng | open | Host-Adapter firewall; integration-first tests on real instances; cross-Office grid; documented threading model | T-0023, T-2051 |
| **R-2** | macOS native add-in may be infeasible (MS pushes Office.js) — biggest threat to Mac parity | High | macOS-Adapter Eng | **open — spike first** | Phase-0 spike T-0001; if infeasible → Office.js fallback w/ documented parity caveats; feeds CL-1 | T-0001, T-3030 |
| **R-3** | Label-placement solver doesn't reach think-cell quality/speed | High | Layout-Solver Eng | open | Build first (Phase 0), oracle-gated; deterministic + incremental; ILP fallback for hard cases | T-0002, T-0016, T-1020 |
| **R-4** | No automatic quality signal for "looks right" — verification is the true bottleneck | High | QA / Verification Lead | open | Oracle (I01) + equivalence harness (I07) are P0, not afterthoughts; corpus grows continuously; Gate-3 human perceptual sign-off | T-B012, T-B014, T-1052 |
| **R-5** | Moving target — think-cell keeps shipping; baseline drifts | Med | Product Owner | mitigated-by-design | Pinned baseline (CL-A); post-baseline features in separate backlog (NG4) | T-B001 |
| **R-6** | IP / legal — close functional clone (patents, trade dress, using competitor binary for reference outputs) | Med–High | Orchestrator | **open — blocks Phase 1** | Get IP counsel before building; clean-room practices; independent algorithm design; care w/ benchmark-output usage. *Not legal advice.* | T-B002, T-B014 |
| **R-7** | Scope — irreducibly large; agent builds demo then stalls on the tail | High | Orchestrator | open | Phased gates; nothing "done" until I06=100% + I07 in tolerance; evidence-bound DoD | all gates |

## Human-checkpoint linkage (Master Part G)

| Checkpoint | Risk(s) | Blocks |
|--|--|--|
| CL-A pin baseline | R-5 | all gates / DoD |
| CL-1 native vs Office.js (post R-2) | R-2 | entire build (Gate 0 exit) |
| R-6 IP/legal posture | R-6 | Phase 1 start |
| Perceptual quality sign-off | R-4 | Gate 3 |
| Release approval | — | Ship |
