# Technical Requirements Document — “Lattice” (Full-Parity Edition)

> **Companion to:** `PRD-lattice.md` v2.0 (Full Parity). This is the Spec Kit **`plan.md`** step: it translates the PRD’s WHAT/WHY into HOW.
> **Status:** Draft for review. Contains **provisional** answers to the PRD’s open questions (§1) — confirm before tasking.
> **Created:** 2026-06-01 · **Version:** 1.0
> **Readability rule (Spec Kit):** this plan stays high-level and navigable. Full IDL, schemas, the solver spec, and OOXML mappings live in referenced `contracts/` and `implementation-details/` files to be generated next. Brief samples only here.

-----

## 0. How to read this document

- Every architectural component traces to a PRD requirement (`FR-*` / `NFR-*` / `C*`). See the traceability table in §4.
- Technology choices carry rationale and a named alternative. Final language/library picks marked **`CONFIRM`** are low-risk and decided at task time.
- The PRD’s `→ TRD` markers are all resolved here or in the referenced detail files.
- **Honesty note:** the architecture below is the one full parity actually requires. It is also the most demanding to build. The Risk (§13) and Complexity-Tracking (§14) sections do not soften that.

-----

## 1. Decisions resolved for this TRD (provisional — confirm)

|PRD open Q                |Provisional decision                                                                                                                                                               |Rationale                                                                                                                                                                                                                                                   |Revisit if                                                                                       |
|--------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------|
|**CL-A** (pin baseline)   |Parity baseline = **think-cell 14.x GA, snapshot 2026-06-01**. v15/Assist **excluded** from v1 parity.                                                                             |Makes “full parity” finite and checkable; v15 is early-access and a moving target.                                                                                                                                                                          |You want AI/translation in the first parity claim.                                               |
|**CL-1** (delivery model) |**Native add-in with a portable shared core**: Windows (COM) + macOS (native), both driving one C++ engine. Office.js only as a **future reach adapter**, never the parity vehicle.|Parity needs deep object-model manipulation, **native-object output for non-users (C1)**, real-time solving (NFR-01), and Win/Mac parity (FR-H03). A pure web app fails C1; Office.js likely can’t hit 100%. This mirrors think-cell’s own proven structure.|The Mac-native spike (§13 R-2) fails — then Mac may be forced onto Office.js with parity caveats.|
|**CL-B** (AI in baseline?)|**No.** Epic J deferred to a post-parity roadmap.                                                                                                                                  |Consistent with pinning to v14; keeps C2 (data-stays-local) clean for v1.                                                                                                                                                                                   |CL-A changes to include v15.                                                                     |
|**CL-9** (commercial)     |Out of TRD scope; a **licensing/activation module** (§6.6) is included as a slot.                                                                                                  |Orthogonal to functional parity; doesn’t block the build.                                                                                                                                                                                                   |—                                                                                                |

-----

## 2. Phase −1 — Pre-implementation gates (Spec Kit constitution)

The constitution’s gates are evaluated honestly; failures are justified in **§14 Complexity Tracking** as the methodology requires.

- **Simplicity Gate (≤3 projects, no future-proofing):** **FAIL.** A full-parity clone of a 20-year product is irreducibly multi-module (see §3). Justified in §14.
- **Anti-Abstraction Gate (use frameworks directly, single model):** **PARTIAL FAIL.** We deliberately wrap the Office object model behind a Host-Adapter interface (§5). Justified in §14 (brittleness + cross-platform are the entire reason the abstraction exists).
- **Library-First (Article I):** **PASS.** The engine is a standalone, host-independent library; every subsystem is a module with a clear boundary.
- **CLI Interface (Article II):** **PASS.** The engine exposes a headless CLI/JSON interface — which simultaneously satisfies automation (FR-E08) and the test oracle (FR-I01/I07). This is a load-bearing design choice, not box-ticking.
- **Test-First / Integration-First (Articles III, IX):** **PASS by mandate** — contracts and contract tests precede implementation; adapter tests run against **real** PowerPoint/Excel instances, not mocks (§10).

-----

## 3. Architecture overview

**Pattern: portable headless core + thin platform host-adapters.** The core computes everything (data, geometry, layout, annotations, style, a platform-neutral scene graph). Adapters do only host I/O: render the scene graph as **native** PowerPoint shapes, read Excel, draw the ribbon/UI, marshal events. This is the single most important decision in the document — it (a) makes Win/Mac parity a function of one engine, (b) quarantines Office brittleness in adapters, (c) makes the hard module (the solver) testable headlessly, and (d) gives automation and the verification oracle the same entry point as the GUI.

```
                ┌──────────────────────────────────────────────────────────┐
                │                  LATTICE CORE  (portable, headless)        │
                │  Data/Datasheet · Chart geometry · LAYOUT SOLVER (moat)    │
                │  Annotations · Style engine · Scene-graph · OOXML codec    │
                │  Calendar/fiscal engine · Self-recognition/round-trip      │
                └───────▲───────────────▲───────────────▲──────────▲────────┘
   Host-Adapter IFC ────┘               │               │          │  Engine API (CLI/lib/RPC)
        ┌────────────────┐   ┌──────────┴─────┐  ┌───────┴──────┐  ┌┴───────────────────────┐
        │ Windows adapter │   │ macOS adapter  │  │ Office.js     │  │ Automation/headless     │
        │ (COM, PPT+XL)   │   │ (native)       │  │ adapter(reach)│  │ (CLI + server: XL/JSON) │
        └────────────────┘   └────────────────┘  └──────────────┘  └─────────────────────────┘
                │  Data connectors: Excel-link engine · Tableau bridge · Chart scanner (CV)
                └────────────────────────────────────────────────────────────────────────────
        Verification: render farm · layout-quality ORACLE · equivalence harness · cross-Office grid
```

**Component → requirement traceability (abridged; full map in `implementation-details/traceability.md`):**

|Component                             |Implements                           |
|--------------------------------------|-------------------------------------|
|Layout solver                         |FR-B01, FR-B02, FR-C12, NFR-01       |
|Chart geometry                        |Epic A, parts of C, D                |
|Annotation engine                     |FR-C01–C08                           |
|Calendar/fiscal engine                |FR-D05                               |
|Scene-graph → native shapes (adapters)|C1, FR-H02, FR-F08, FR-I05           |
|OOXML codec + self-recognition        |FR-H02, round-trip, FR-E04           |
|Excel-link engine + registry          |FR-E02, E03, E05, E06                |
|Automation/headless                   |FR-E08, FR-H06                       |
|Style engine                          |FR-F01, F02; Epic-wide brand defaults|
|Verification harness                  |Epic I (I01–I07)                     |

-----

## 4. Core engine design (the product’s center of gravity)

Modules (each a library with its own contract tests):

1. **Data & Datasheet engine** — in-memory spreadsheet model; series/category/value model; number-format-by-example (FR-C05); paste/coercion rules; the “editable data layout” interpretation logic (FR-E03). Deterministic.
1. **Chart geometry** — pure functions mapping `(data, chart-type, style, area)` → primitive shapes for every type in FR-A04 + Gantt (Epic D). No I/O. Output feeds both the solver and the scene-graph.
1. **Layout constraint solver (keystone, FR-B01/B02).** Two coupled problems:
- *Continuous layout* (element/area sizing, alignment, locks): a **linear-constraint solver** (Cassowary-style) is a strong fit. `CONFIRM`.
- *Discrete label placement* (inside-segment vs. above vs. external-with-leader-line, collision-free): a **map-labeling / optimization** problem (NP-hard in general). Approach: enumerate candidate slots per label → score (overlap, distance, legibility) → solve via incremental search (greedy + local repair / simulated annealing / ILP for hard cases). **Hard requirements: deterministic** (same input → same output, for FR-I07 equivalence) and **incremental** (fast re-solve on data change). Leader-line routing must avoid crossing other elements.
- Full spec → `implementation-details/layout-solver.md`. This module is built and de-risked **first** (PRD Phase 0).
1. **Annotation engine** — computes CAGR/difference/value/net-line values and their geometry, then hands placement to the solver (FR-C01–C08, C12).
1. **Style engine** — parses style files (FR-F01), resolves theme shades, applies brand defaults to new elements, supports preset chart layouts (FR-B11). Style-file format spec → `contracts/style-file.md`.
1. **Scene-graph / display list** — the platform-neutral output: typed primitives (`Rect`, `Path/Freeform`, `Line/Connector`, `TextRun`, `Group`, fills, strokes, z-order). Adapters translate 1:1 to native shapes. Decouples computation from rendering and is what makes C1 possible.
1. **OOXML codec + self-recognition** — emits native PPTX shapes; stamps Lattice metadata into **Custom XML Parts + shape tags** keyed to shape IDs so the engine can re-parse and re-edit its own elements; defines the reconcile policy when a user manually edits a generated shape. Without the add-in, only the native group remains — visible and editable (C1, FR-H02).
1. **Calendar/fiscal engine** — 4-4-5 / 4-5-4 / 5-4-4, leap weeks, weekday-aligned fiscal year-ends (FR-D05). Pure, highly testable.

**Language:** **C++20** core (COM-ecosystem fit, performance, mature 2D-geometry libs, mirrors think-cell). **Alternative:** Rust core behind a C ABI (memory-safety upside for NFR-02; weaker Office interop). `CONFIRM`. Build: CMake, single source tree, platform conditionals only in adapters.

-----

## 5. Host integration adapters (thin; the brittleness firewall)

**Host-Adapter interface (the contract the core depends on)** — sketch; full IDL → `contracts/host-adapter.md`:

```
createShapes(sceneGraph) -> shapeIds        // materialize as NATIVE PowerPoint shapes
readRange(workbook, sheet, range) -> grid   // for Excel links
onSelection / onSlideChange / onEdit(...)   // host events into the core
readDocumentMetadata() / writeDocumentMetadata(...)   // custom XML parts
```

- **Windows adapter** — native **C++ COM** add-in (Office `IDTExtensibility2` / add-in model) for **PowerPoint and Excel**; ribbon via Office ribbon XML; datasheet UI as a lightweight embedded window. Native C++ (not VSTO/.NET) to avoid CLR marshaling and match think-cell. **Threading:** Office COM is **STA**; the solver runs on a worker, results marshaled to the UI thread under documented rules (a real source of bugs — see R-1).
- **macOS adapter** — native Mac Office integration. **This is the highest-risk component**: modern Office for Mac steers third parties toward Office.js, and native Mac add-in capability is constrained. If the §13 R-2 spike shows native is infeasible, Mac falls back to an **Office.js adapter with documented parity caveats** — a split we accept only if forced.
- **Office.js adapter (reach, post-parity)** — same core, reduced fidelity; explicitly *not* the parity vehicle.

**Graceful degradation (C1):** because adapters emit native grouped shapes (not pictures/OLE), a Lattice-free PowerPoint shows correct, editable output. Validated by FR-I05.

-----

## 6. Data layer, connectors & services

1. **Excel-link engine (FR-E02/E03/E05/E06).** A **link registry** in the document’s custom XML maps each element’s data to `(workbook, sheet, range, layout)` with state `connected | stale | broken` and an update policy. Refresh reads via the host’s Excel object model (interactive) or the OOXML reader (headless). Handles moved/renamed sources, manual recalculation, disconnected-link cleanup, source-change alerts + alternate-source selection.
1. **Slide workbook (FR-E04).** An embedded workbook part feeding all data-driven elements on a slide; engine treats it as a first-class datasheet source.
1. **Tableau connector (FR-E07).** Browser-extension capture → data bridge → engine; stores the binding for re-update. Contract → `contracts/connectors.md`.
1. **Automation / headless (FR-E08, FR-H06).** The core compiled as a CLI + a server (gRPC or REST `CONFIRM`) consuming a **template .pptx + data (Excel *and* JSON)** → emits .pptx. Named-text-field placeholders; fixed chart width on category-count change. Reuses the core directly — no host app needed.
1. **Chart scanner (FR-E09).** Separable CV pipeline: segmentation → axis/scale detection → OCR → datasheet. A vision-model-backed service is viable; isolated so it never blocks the core.
1. **Licensing/activation slot (CL-9).** Pluggable; no functional coupling to the engine.

-----

## 7. Data models (high-level; full schemas → `implementation-details/data-model.md`)

**Entities:** `Element`(geometry, locks, styleRef, kind) · `Chart`(series[], categories[], datasheetRef, axes, labels, annotations[]) · `Gantt`(activities[], milestones[], brackets[], processes[], dateScale, fiscalCalendar, links[]) · `Datasheet`(grid, formats, source: Internal|SlideWorkbook|ExternalLink) · `ExternalLink`(target, state, policy) · `Annotation`(type, computedValue, geometry) · `StyleFile`(colors, fonts, schemes, lineStyles, ganttStyle, fiscalCalendars, agendaLayouts, presetLayouts) · `SceneGraphNode`(primitive union) · `DocumentMetadata`(custom-XML schema mapping shapeIds→model). All core models are **serializable and deterministic** to support equivalence testing.

-----

## 8. Contracts / interfaces (interface-level; full set → `contracts/`)

1. **Engine API** — the central contract (create/edit/query elements; recompute; emit scene graph; import/export). Used identically by adapters, CLI, server, and tests.
1. **Host-Adapter interface** — §5.
1. **CLI contract** — text/JSON in, .pptx/JSON out (Article II).
1. **Automation server API** — template + data → deck.
1. **Connector contracts** — Excel, Tableau, scanner.
1. **Style-file format** — public, documented (FR-F01).

Test-first ordering means these contracts and their **contract tests are written before implementation** (§10).

-----

## 9. Technology stack (the deferred HOW)

|Layer                |Choice                                            |Alternative / note                  |
|---------------------|--------------------------------------------------|------------------------------------|
|Core engine          |C++20, CMake                                      |Rust + C ABI (`CONFIRM`)            |
|Continuous layout    |Cassowary-style linear solver                     |custom                              |
|Label placement      |candidate-slot search + local repair / SA / ILP   |core IP; deterministic & incremental|
|Windows adapter      |native C++ COM add-in                             |VSTO/.NET (rejected: marshaling)    |
|macOS adapter        |native (else Office.js fallback)                  |**R-2 spike decides**               |
|OOXML                |direct OOXML manipulation + Custom XML Parts      |—                                   |
|Excel read (headless)|OOXML reader lib                                  |host object model (interactive)     |
|Automation           |CLI + gRPC/REST server                            |`CONFIRM`                           |
|Chart scanner        |CV service (vision-model-backed)                  |classical CV                        |
|Verification         |headless render + geometry/perceptual diff harness|§10                                 |

-----

## 10. Test & verification strategy (test-first, integration-first — and the parity-proof machinery)

Ordering (Spec Kit Article III): **contracts → contract tests → integration tests → e2e → unit → implementation that makes them pass.**

- **Layout-quality oracle (FR-I01)** — headless: render engine output, score **label-overlap (gate: 0)**, legibility, alignment, balance. Runs in CI on every change. The keystone test asset.
- **Equivalence harness (FR-I07)** — given identical inputs, diff Lattice output vs. the **pinned benchmark’s** output (visual + behavioral) within tolerance. *Requires reference outputs generated from think-cell in a controlled test env — see the IP/legal flag, R-6.*
- **Regression corpus (FR-I02)** — real decks + every §5.2 adversarial case; each fixed bug adds a fixture.
- **Collision gate (FR-I03)** — overlap > 0 blocks release.
- **Cross-Office grid (FR-I04)** — automated runs across supported Office versions / bitness / OS; catches the undocumented-behavior failures.
- **Round-trip + non-user-open (FR-I05)** — open→save→reopen, and open in Lattice-free PowerPoint, preserve appearance + editability (C1).
- **Integration-first (Article IX):** adapter tests run against **real** PowerPoint/Excel instances; mocks only below the adapter boundary.
- **Parity checklist (FR-I06)** — the acceptance ledger; **Done = 100%** (PRD §12).

-----

## 11. Build sequencing (maps to PRD Phases; `[P]` = parallelizable across agents)

- **Phase 0 — Moat + feasibility.** Engine skeleton + Engine API contract; **layout solver** for stacked-column + waterfall; datasheet; auto-totals; one CAGR arrow; **oracle (I01) live**; Windows adapter spike for native-shape emission; **R-2 Mac-native spike**. *Gate: 0 overlaps on ~50 fixtures; CL-1 confirmed on evidence.*
- **Phase 1 — Charting + layout core.** `[P]` chart-geometry (Epic A) · `[P]` annotation engine (C01–C06) · `[P]` Excel-link engine (E01–E03) · scene-graph→shapes hardening · style engine v1 (F01) · gates I01–I03, I07 v1. *Gate: Scenarios A–D; <60 s reference chart.*
- **Phase 2 — Breadth.** `[P]` remaining annotations (C07–C11) · `[P]` **Gantt + fiscal engine** (Epic D) · `[P]` slide workbooks/linked tables/linked text (E04–E06) · `[P]` brand & hygiene (Epic F) · conditional-formatting import (E11) · RTL (H04) · cross-Office grid (I04) · round-trip (I05) · parity checklist stood up (I06). *Gate: Scenarios E (interactive) & F; parity coverage trending to 100%.*
- **Phase 3 — Full surface.** `[P]` automation CLI/server + API (E08, H06) · `[P]` chart scanner (E09) · `[P]` Tableau (E07) · `[P]` Library (Epic G) · co-authoring (H08) · macOS adapter to full parity. *Gate: **I06 = 100%; I07 within tolerance; C1 verified** → parity declared.*

Agent decomposition follows the module boundaries (§4–§6); the Engine API and Host-Adapter contracts are the seams that let agents work in parallel without integration drift.

-----

## 12. Cross-cutting concerns

- **Performance (NFR-01):** solver in native code; **incremental** re-solve; worker-thread compute with careful STA marshaling; target bounds set in `implementation-details/perf-budgets.md` (`CONFIRM` exact numbers, ≥ benchmark).
- **Reliability (NFR-02):** defensive COM lifetime/exception handling in adapters; the core is pure/deterministic and fuzz-tested; never corrupt the host document.
- **Security/Privacy (NFR-04, C2):** files local by default; auditable network calls; sanitize/clean-up (FR-F03) for data-leak prevention.
- **Localization/RTL (C3, H04):** all formatting localizable; RTL handled in geometry + scene-graph, flipped on language change.
- **Observability (NFR-07):** structured logs + reproducible-state capture for support.

-----

## 13. Risks & mitigations

|ID     |Risk                                                                                                                                                                                                                   |Severity|Mitigation                                                                                                                                                                                                                                          |
|-------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|**R-1**|**Office object-model brittleness / COM-STA threading** — the classic failure surface; bugs appear only on real Office, not in unit tests.                                                                             |High    |Host-Adapter firewall (§5); integration-first tests on real instances; cross-Office grid (I04); documented threading model.                                                                                                                         |
|**R-2**|**macOS native add-in may be infeasible** on current Office for Mac (MS pushes Office.js).                                                                                                                             |High    |Phase-0 spike; if infeasible, Mac on Office.js with documented parity caveats. **This is the biggest threat to the parity claim on Mac.**                                                                                                           |
|**R-3**|**Label-placement solver** doesn’t reach think-cell quality / speed.                                                                                                                                                   |High    |Build first (Phase 0); oracle-gated; deterministic + incremental design; ILP fallback for hard cases.                                                                                                                                               |
|**R-4**|**No automatic quality signal** for “looks right” → verification is the true bottleneck (as argued throughout).                                                                                                        |High    |The oracle (I01) + equivalence harness (I07) are P0, not afterthoughts; corpus grows continuously.                                                                                                                                                  |
|**R-5**|**Moving target** — think-cell keeps shipping; baseline drifts.                                                                                                                                                        |Med     |Pinned baseline (CL-A); post-baseline features in a separate backlog.                                                                                                                                                                               |
|**R-6**|**IP / legal** — a close functional clone raises questions around patents (layout algorithms), trade dress, verbatim expression, and using the competitor’s binary to generate reference outputs for equivalence tests.|Med–High|**Get IP counsel before building.** Functionality/ideas aren’t copyrightable, but clean-room practices, independent algorithm design, and care with trade dress and benchmark-output usage matter. *Not legal advice; flagging a real project risk.*|
|**R-7**|**Scope** — irreducibly large; agent builds demo well then stall on the tail.                                                                                                                                         |High    |Phased gates; nothing declared done until I06=100% + I07 in tolerance.                                                                                                                                                                             |

-----

## 14. Complexity Tracking (justified exceptions to the constitution)

- **Simplicity Gate (≤3 projects) — exception granted.** Justification: full parity with a decade-plus product spans ≥6 genuinely separable subsystems (engine, Windows adapter, Mac adapter, connectors, automation, verification). Collapsing them would couple the brittle host code to the pure engine and break headless testing. Mitigation: strict module boundaries + contract tests keep effective complexity bounded.
- **Anti-Abstraction Gate — exception granted (Host-Adapter layer).** Justification: the abstraction *is* the mitigation for R-1 and the enabler of Win/Mac parity and headless testing. Using the Office API “directly” everywhere would spread brittleness through the whole codebase. The abstraction is thin (I/O only); the core never sees COM.
- All other articles (library-first, CLI, test-first, integration-first) are honored as designed.

-----

## 15. Open technical questions (residual; resolve during Phase 0)

- **TQ-1** CL-1 confirmation hinges on **R-2** (Mac-native spike). Run it first.
- **TQ-2** Layout-solver technique selection (search vs. ILP vs. hybrid) — decide from Phase-0 quality/perf data.
- **TQ-3** Exact performance budgets (NFR-01) — set numerically once the solver baseline exists.
- **TQ-4** Reconcile policy for user-edited generated shapes (round-trip) — define precisely.
- **TQ-5** Core language final (C++ vs Rust) — `CONFIRM`.
- **TQ-6** R-6 IP posture — counsel input before Phase 1.

-----

## 16. Next Spec Kit steps

1. Confirm §1 decisions (CL-A, CL-1, CL-B) and resolve §15.
1. Generate detail files: `contracts/` (engine-api, host-adapter, connectors, style-file), `implementation-details/` (layout-solver, data-model, traceability, perf-budgets), and `research.md` (R-2 + solver-technique spike findings).
1. Generate **`tasks.md`** — the parallelizable agent task list derived from the module boundaries and contracts above, with `[P]` markers and test-first ordering.

-----

*End of TRD. The architecture is the one full parity requires; the verification machinery is the part that determines whether parity is reached, not merely attempted.*
