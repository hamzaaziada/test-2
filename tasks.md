# tasks.md — “Lattice” Full-Parity Build Backlog

> **Spec Kit step:** `tasks.md`. Derived mechanically from `TRD-lattice.md` §4–§11 (module boundaries + contracts) and `PRD-lattice.md` (FR-*/NFR-*). This is the dependency-ordered, `[P]`-tagged, test-first task list the engineering company (`MASTER-BUILD-PROMPT.md`) executes.
> **Created:** 2026-06-01 · **Version:** 1.0

-----

## 0. How to read this backlog

- **ID** — `T-<phase><nnn>`. Phase `B` = bootstrap/Phase-−1, then `0/1/2/3`.
- **`[P]`** — parallelizable: no dependency on a sibling `[P]` task in the same group; different agents can run them concurrently. Tasks **without** `[P]` are on a critical path within their group.
- **Test-first ordering (TRD §10, Master Directive 3):** every implementation task is preceded by its **contract task** and a **contract-test task that must be RED before implementation starts**. Contract-test tasks are marked `(red-first)`.
- **Owner** — role from `MASTER-BUILD-PROMPT.md` Part B.
- **Traces** — the FR/NFR/C and the `parity-ledger.md` rows the task satisfies. *No task is "done" without ledger evidence (Directive 1).*
- **Gate** — the stage gate (Part D) this task must clear into.
- **DoD** — task is done only when: contract tests green + implementation green + integration test on real Office (where applicable) + oracle coverage added (where geometry is produced) + Adversarial + peer review + ledger row updated.
- A task that **produces geometry/layout** is BLOCKED until its oracle fixtures exist (Directive 2).

Legend: 🔴 = human checkpoint (Part G) blocks this task · 🧱 = critical-path keystone · 🔬 = spike (throwaway, never merged as product).

-----

## Phase −1 — Bootstrap (sets up the spine before any feature)

> Exit: contracts v0 published with **red** contract tests, CI + oracle skeleton live, corpus seeded, human checkpoints CL-A queued. Maps to Master Part F bootstrapping.

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-B001|🔴|**Pin parity baseline (CL-A).** Record exact think-cell version + snapshot date in `parity-ledger.md` header. *Blocks all gates.*|Product Owner|—|§1.4, FR-I06|
|T-B002|🔴|**Queue IP/legal counsel (R-6).** Open risk-register item; gate Phase-1 start on counsel input, esp. equivalence-harness use of competitor output.|Orchestrator|—|R-6, NFR-04|
|T-B003||**Stand up `parity-ledger.md`:** one row per FR (A01…J04), each mapped to an acceptance-test ID and a status (`not-started/red/green/parity`).|Product Owner|—|FR-I06|
|T-B004||**Stand up `risk-register.md`** (R-1…R-7 live) and **`complexity-tracking.md`** (TRD §14 exceptions ratified).|Orchestrator|—|TRD §13,§14|
|T-B005|🧱|**Author Engine API contract v0** (`contracts/engine-api.md`): create/edit/query elements, recompute, emit scene-graph, import/export.|Principal Architect|T-B003|TRD §8.1, FR-H06|
|T-B006|🧱|**Author Host-Adapter contract v0** (`contracts/host-adapter.md`): `createShapes/readRange/onEvents/read+writeDocumentMetadata`.|Principal Architect|T-B005|TRD §5,§8.2|
|T-B007|`[P]`|**Author CLI contract** (`contracts/cli.md`): text/JSON in → pptx/JSON out.|Automation Eng|T-B005|TRD §8.3, FR-E08|
|T-B008|`[P]`|**Author Style-file format contract** (`contracts/style-file.md`).|Style Eng|T-B005|TRD §8.6, FR-F01|
|T-B009|`[P]`|**Author connector contracts** (`contracts/connectors.md`): Excel / Tableau / scanner.|Connectors Eng|T-B005|TRD §8.5, FR-E02/E07/E09|
|T-B010|`(red-first)`|**Contract tests for Engine API + Host-Adapter** — written, committed **RED**. No implementation may start until these exist.|SDET|T-B005,T-B006|Directive 3|
|T-B011|🧱|**CI pipeline skeleton** (`.ci/`): build matrix stub, runs contract tests, blocks merge on red (Part E checks).|SDET|T-B010|FR-I04, Part E|
|T-B012|🧱|**Layout-quality oracle skeleton** (`verification/oracle/`): ingest scene-graph → score overlap (gate 0), legibility, alignment, balance. Overlap metric must be live before any geometry task.|QA / Verification Lead|T-B005|FR-I01, FR-I03|
|T-B013||**Seed regression corpus** (`verification/corpus/`) with every §5.2 adversarial case as an empty/expected fixture.|QA / Verification Lead|T-B012|FR-I02, PRD §5.2|
|T-B014||**Equivalence-harness skeleton** (`verification/equivalence/`): input → output diff (visual+behavioral) within tolerance; tolerance config stub. 🔴 gated by R-6 before using competitor reference output.|QA / Verification Lead|T-B012, T-B002|FR-I07|
|T-B015||**ADR log bootstrap** (`adr/0000-template.md`) + open the standing ADRs listed in Master Part C1.|Principal Architect|—|Part C1|

-----

## Phase 0 — Moat & Feasibility  → **GATE 0**

> Build the hardest thing first. Exit evidence (Gate 0): solver clears **0 overlaps on ~50 Phase-0 fixtures** (oracle live in CI); Windows adapter emits **native** shapes for a stacked column; **R-2 Mac spike delivered + CL-1 confirmed by human**. Solver failing its gate halts the project. (PRD §11 Phase 0; TRD §11.)

### 0.1 Spikes (parallel, timeboxed, throwaway — Master Part C4)

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-0001|`[P]` 🔬🔴|**R-2 macOS-native feasibility spike.** Can a native Mac Office add-in emit native shapes + host UI? Written question + success criteria → spike report → ADR. Feeds CL-1.|macOS-Adapter Eng|T-B006|R-2, TRD TQ-1, FR-H03|
|T-0002|`[P]` 🔬|**Solver-technique spike.** Candidate-slot search vs. ILP vs. hybrid for collision-free label placement; measure quality + re-solve latency → ADR (TQ-2).|Layout-Solver Eng|T-B012|FR-B01, R-3, TQ-2|
|T-0003|`[P]` 🔬|**Native-shape emission spike.** Shape-by-shape vs. grouped-freeform for C1 fidelity → ADR.|OOXML Eng|T-B006|C1, FR-H02|
|T-0004|`[P]` 🔬|**Core-language spike** (C++20 vs Rust+C-ABI) → ADR (TQ-5). Low-risk; decided at task time.|Principal Architect|T-B005|TRD §4, TQ-5|

### 0.2 Engine skeleton + keystone solver (critical path)

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-0010||**Engine skeleton + Engine API impl stub** that turns contract tests from red→green for create/query/emit-scene-graph.|Principal Architect|T-B010,T-0004|Engine API|
|T-0011|`(red-first)`|**Datasheet contract tests** (paste/coercion, auto-totals, used-range growth) — RED.|SDET|T-0010|FR-A02,A03|
|T-0012||**Data & Datasheet engine v0** (internal grid, series/category/value, auto totals + grand totals). Deterministic.|Data/Datasheet Eng|T-0011|FR-A02,A03,E01|
|T-0013|`(red-first)`|**Chart-geometry contract tests** for stacked column + waterfall (incl. subtotal/total columns hold position) — RED.|SDET|T-0010|FR-A04|
|T-0014||**Chart geometry v0**: stacked column + waterfall → primitive shapes (pure, no I/O).|Chart-Geometry Eng|T-0013,T-0012|FR-A04, A07|
|T-0015|🧱 `(red-first)`|**Solver contract + oracle fixtures (~50 cases)**: stacked column + waterfall label scenarios incl. tiny-segment→leader-line. Fixtures RED in oracle.|QA + Layout-Solver Eng|T-B012,T-0002|FR-B01, FR-I02|
|T-0016|🧱|**Layout solver v1 — collision-free label placement** (inside/above/external-with-leader-line) for stacked column + waterfall. **Deterministic + incremental.** Leader-line routing avoids crossings.|Layout-Solver Eng|T-0015,T-0014|FR-B01, FR-C12, NFR-01|
|T-0017||**Continuous-layout (Cassowary-style) v0**: area sizing/alignment honoring locks, feeding the solver.|Layout-Solver Eng|T-0016|FR-B02, B10|
|T-0018||**One CAGR arrow (FR-C01)**: compute value + geometry; placement via solver; `p.a.` suffix.|Annotations Eng|T-0016|FR-C01|

### 0.3 Scene-graph → native shapes + Windows spike

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-0020||**Scene-graph / display list v0**: typed primitives (Rect/Path/Line/TextRun/Group, fills, strokes, z-order).|OOXML Eng|T-0010|TRD §4.6, C1|
|T-0021||**OOXML codec v0 + self-recognition**: emit native PPTX shapes; stamp Lattice metadata (Custom XML Parts + shape tags keyed to shape IDs).|OOXML Eng|T-0020,T-0003|FR-H02, FR-E04|
|T-0022|`(red-first)`|**Windows adapter integration test (real PowerPoint)** for native-shape emission of a stacked column — RED.|SDET|T-B006|FR-H01, Integration-first|
|T-0023|🧱|**Windows adapter spike→v0**: `createShapes` materializes the stacked-column scene-graph as **native** shapes in real PowerPoint (STA worker→UI marshaling documented).|Windows-Adapter Eng|T-0022,T-0021|FR-H01, R-1|

### 0.4 Gate 0 evidence

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-0030||**Wire oracle into CI as a hard gate** (overlap > 0 blocks). Run the ~50 fixtures.|SDET|T-0016,T-B011|FR-I03|
|T-0031|🔴|**GATE 0 review.** Evidence: 0 overlaps on the 50-case set; native-shape emission demonstrated on real PPT; R-2 report delivered; **human confirms CL-1**. Record result; update ledger + ADRs.|Orchestrator + gatekeepers|T-0030,T-0023,T-0001|PRD §11, Part D|

-----

## Phase 1 — Charting + Layout Core  → **GATE 1**

> 🔴 **Blocked until R-6 (IP counsel) cleared** (Part G). Exit (Gate 1): Scenarios A–D pass on real Office; **<60 s** reference chart; **overlap = 0** on corpus; Excel links refresh; equivalence harness v1 within tolerance for tier-1 charts. (PRD §11 Phase 1.)

### 1.1 Chart catalogue breadth (Epic A) — `[P]` across chart families

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-1001|`(red-first)`|Geometry contract tests for all remaining FR-A04 types — RED.|SDET|T-0014|FR-A04|
|T-1002|`[P]`|**Column/bar family**: clustered column, stacked/clustered bar (+ rotated variants).|Chart-Geometry Eng|T-1001|FR-A04, A07|
|T-1003|`[P]`|**Line/area family**: line, area, stacked area, profile/rotated line.|Chart-Geometry Eng|T-1001|FR-A04|
|T-1004|`[P]`|**100% charts** (column/area) + **absolute↔percentage toggle** without visible axis.|Chart-Geometry Eng|T-1001|FR-A04, A05|
|T-1005|`[P]`|**Mekko/Marimekko** (percentage axis + grand total).|Chart-Geometry Eng|T-1001|FR-A04|
|T-1006|`[P]`|**Pie + doughnut** (sort order).|Chart-Geometry Eng|T-1001|FR-A04|
|T-1007|`[P]`|**Scatter + bubble** (≥4,000 points — perf-aware).|Chart-Geometry Eng|T-1001|FR-A04, C4|
|T-1008|`[P]`|**Combination/combo** (+ rotated combo).|Chart-Geometry Eng|T-1001|FR-A04|
|T-1009||**Chart-type conversion** post-creation (stacked↔clustered, segments↔line).|Chart-Geometry Eng|T-1002,T-1003|FR-A05|
|T-1010||**Insert/selection UX**: ribbon insert + snapping/alignment; progressive “select-wider”; contextual mini-toolbar; direct-handle width/gap/segment-order/baseline.|Windows-Adapter Eng|T-0023|FR-A01, A06, A07|

### 1.2 Layout & smart elements (Epic B) — solver generalization

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-1020|🧱|**Generalize solver** to all Phase-1 chart types; per-type candidate-slot models; corpus fixtures per type.|Layout-Solver Eng|T-0016,T-1002..T-1008|FR-B01, B02|
|T-1021|`[P]`|**Smart text boxes** that participate in layout (push/resize neighbors).|Layout-Solver Eng|T-1020|FR-B03|
|T-1022|`[P]`|**Element lock model** (fix size/position; lock-by-default option).|Layout-Solver Eng|T-1017?→T-0017|FR-B10|
|T-1023|`[P]`|**Align / match-size / swap / scale-together / group / rotate-flip / arrange** layout tools.|Chart-Geometry Eng|T-1020|FR-B09|
|T-1024|`[P]`|**Same-scale linking** across charts (persistent; mix-and-match anchor; reset).|Layout-Solver Eng|T-1020|FR-B12|

### 1.3 Annotations C01–C06 + C12 — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-1030|`(red-first)`|Annotation value+geometry contract tests — RED.|SDET|T-0018|FR-C01–C06|
|T-1031|`[P]`|**Difference arrows** (absolute and/or %).|Annotations Eng|T-1030|FR-C02|
|T-1032|`[P]`|**Value lines** (auto mean/median).|Annotations Eng|T-1030|FR-C03|
|T-1033|`[P]`|**Net lines** (per-category benchmark overlay).|Annotations Eng|T-1030|FR-C04|
|T-1034|`[P]`|**Label system** (series/category/segment/grand-total; **number-format-by-example**; magnitude/units; rotation; persistent font color; 255-char; selective 4-digit year).|Data/Datasheet Eng|T-1030|FR-C05|
|T-1035|`[P]`|**Axes** (value/category/ordinal/percentage; axis breaks; reversed; gridlines-only; sort-by-total).|Chart-Geometry Eng|T-1030|FR-C06|
|T-1036||**C12 wiring**: all annotations reposition automatically via solver on data/geometry change.|Annotations Eng|T-1031..T-1035,T-1020|FR-C12|

### 1.4 Excel links E01–E03 + style v1 + degradation

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-1040|`(red-first)`|Excel-link engine contract tests (link/refresh/auto-update; moved/renamed source) — RED.|SDET|T-B009|FR-E02|
|T-1041|🧱|**Excel-link engine + registry** (custom-XML map; state connected/stale/broken; link-management dialog: list/locate/repair/highlight/update-revert/select-by-source/delete-disconnected/alert+alternate-source).|Connectors Eng|T-1040,T-0023|FR-E02|
|T-1042|`[P]`|**Editable data layout** (toggle series/labels/totals; link to existing sheets w/o restructuring).|Connectors Eng|T-1041|FR-E03|
|T-1043|`[P]`|**Style engine v1** (parse style file; theme shades base+brightness; brand defaults; preset chart layouts; default-displayed labels).|Style Eng|T-B008,T-0020|FR-F01, B11|
|T-1044||**Scene-graph→shapes hardening** for full catalogue; **graceful-degradation** path (native grouped shapes only).|OOXML Eng|T-0021,T-1002..T-1008|C1, FR-H02, F08|

### 1.5 Gate 1 evidence

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-1050|`(red-first)`|**Scenario A–D acceptance tests on real Office** — RED, then green: scribble→chart; data-change refresh; non-user open; annotated growth story.|SDET + QA|T-1010,T-1036,T-1041,T-1044|PRD §5.1 A–D|
|T-1051||**<60 s reference-chart benchmark** test in CI.|Performance Eng|T-1050|North-star, NFR-01|
|T-1052||**Equivalence harness v1** within tolerance for tier-1 charts (stacked column, waterfall, line, 100%).|QA / Verification Lead|T-B014,T-1020|FR-I07|
|T-1053|🔴|**GATE 1 review.** Evidence: A–D pass; <60 s; overlap = 0 on corpus; links refresh; equivalence v1 in tolerance; all test-first + reviewed.|Orchestrator + gatekeepers|T-1050,T-1051,T-1052|Part D Gate 1|

-----

## Phase 2 — Breadth  → **GATE 2**

> Exit (Gate 2): Scenarios E (interactive) & F pass; Gantt + fiscal calendars correct; conditional-formatting import; RTL; **cross-Office grid green**; round-trip + non-user-open (C1) pass; parity-ledger trending to 100%; security sign-off (no egress). (PRD §11 Phase 2.)

### 2.1 Remaining annotations C07–C11 — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-2001|`[P]`|**Error bars → football-field** charts.|Annotations Eng|T-1036|FR-C07|
|T-2002|`[P]`|**Trendlines** (linear/poly q-c-quartic/log/exp/power) + **partitions** with fills (incl. non-linear-bounded regions).|Annotations Eng|T-1036|FR-C08|
|T-2003|`[P]`|**Legends** (order by datasheet/visual/alphabetical; highlight-on-select).|Chart-Geometry Eng|T-1036|FR-C09|
|T-2004|`[P]`|**Tooltips** (label + value per datapoint).|Windows-Adapter Eng|T-1010|FR-C10|
|T-2005|`[P]`|**“Other”-series aggregation** via drag handle; datasheet highlight; toggle inclusion.|Data/Datasheet Eng|T-1034|FR-C11|

### 2.2 Gantt + fiscal (Epic D) — `[P]` pod

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-2010|`(red-first)`|Calendar/fiscal contract tests (4-4-5/4-5-4/5-4-4, leap weeks, weekday-aligned year-ends) — RED.|SDET|T-B005|FR-D05|
|T-2011|🧱|**Calendar/fiscal engine** (pure, deterministic).|Calendar/Fiscal Eng|T-2010|FR-D05|
|T-2012||**Gantt geometry**: activities/milestones/brackets/processes; date scale day→year (style-driven).|Chart-Geometry Eng|T-2011,T-1043|FR-D01, D02|
|T-2013|`[P]`|**Dependency/link lines** between activities (routed via solver).|Layout-Solver Eng|T-2012|FR-D03|
|T-2014|`[P]`|**Excel-linked dates** (activities/milestones update on change).|Connectors Eng|T-2012,T-1041|FR-D04|
|T-2015|`[P]`|**Gantt RTL + move-all-date-items.**|Chart-Geometry Eng|T-2012|FR-D06|

### 2.3 Data layer E04–E06, E10–E11 — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-2020|`[P]`|**Slide workbook** (embedded workbook part feeding all data-driven elements).|Connectors Eng|T-1041,T-0021|FR-E04|
|T-2021|`[P]`|**Linked data tables** (table↔Excel auto-update).|Connectors Eng|T-2020|FR-E05, B06|
|T-2022|`[P]`|**Text fields linked to Excel cells** (embeddable in any label/text box).|Connectors Eng|T-2020|FR-E06|
|T-2023|`[P]`|**Excel tools**: CAGR calc; table/cell styles; data rounding.|Style Eng|T-1043|FR-E10|
|T-2024|`[P]`|**Conditional-formatting import** (color scales; cell-ref/text/date rules → charts+tables).|Connectors Eng|T-2021|FR-E11|

### 2.4 Smart elements B04–B08 + Tables — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-2030|`[P]`|**Process flows** (chevron/pentagon; duplication; H↔V re-flow).|Layout-Solver Eng|T-1020|FR-B04|
|T-2031|`[P]`|**Agendas** (multi-level TOC sync; flat style; hide-level; default layouts).|Layout-Solver Eng|T-1043|FR-B05|
|T-2032|`[P]`|**Tables** with datasheet (auto format/size, stripes, cell styles).|Chart-Geometry Eng|T-2021|FR-B06|
|T-2033|`[P]`|**Harvey balls & checkboxes** (data-linked; custom images; non-square).|Style Eng|T-2022|FR-B07|
|T-2034|`[P]`|**Rounded rectangles** as managed elements + **preset slide layouts / thumbnails**.|Layout-Solver Eng|T-1043|FR-B08, B11|

### 2.5 Brand & hygiene (Epic F) — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-2040|`[P]`|**Style-file full surface** (line styles, arrow fills, gantt styles, fiscal-cal defs, agenda layouts, legend fonts, scheme-not-remembered).|Style Eng|T-1043,T-2011|FR-F01, F02|
|T-2041|`[P]` 🔒|**Clean up / sanitize** (remove comments/notes/unused masters/animations/links; **randomize all numbers**).|Security Reviewer + Style Eng|T-2020|FR-F03, C2|
|T-2042|`[P]`|**Send/Save slides** (export selected/all; email incl. Gmail; filename date/time). 🔒 egress reviewed.|Connectors Eng|T-2041|FR-F04|
|T-2043|`[P]`|**Productivity tools** (decimal symbol; replace/resize fonts; proofing language; insert symbols; remove animations).|Style Eng|T-2040|FR-F05|
|T-2044|`[P]`|**Color picker** (live preview, hex/RGB, eyedropper) + **file-name/path on slides**.|Windows-Adapter Eng|T-1010|FR-F06, F07|

### 2.6 RTL + verification gates

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-2050||**RTL (Arabic/Hebrew)** in geometry + scene-graph; auto-flip on language change; RTL flip of existing chart.|Chart-Geometry Eng|T-1020,T-2015|FR-H04, C3|
|T-2051|🧱|**Cross-Office grid (FR-I04)**: automated runs across Office versions/bitness/OS (incl. Win ARM, 32/64-bit).|SDET|T-B011,T-0023|FR-I04, NFR-03|
|T-2052|🧱|**Round-trip + non-user-open fidelity (FR-I05)**: open→save→reopen + Lattice-free open preserve appearance + editability.|QA / Verification Lead|T-1044,T-0021|FR-I05, C1|
|T-2053||**Parity checklist stood up (FR-I06)** with measured coverage trending to 100%.|Product Owner|T-B003|FR-I06|
|T-2054|`(red-first)`|**Scenario E (interactive) & F acceptance tests** — RED→green.|SDET + QA|T-2014,T-2012,T-2024|PRD §5.1 E,F|
|T-2055|🔴 🔒|**GATE 2 review.** Evidence per Part D Gate 2 incl. security sign-off (no egress).|Orchestrator + gatekeepers|T-2050,T-2051,T-2052,T-2054|Part D Gate 2|

-----

## Phase 3 — Full Surface → **GATE 3 (parity declared)**

> Exit (Gate 3 / DoD): **FR-I06 = 100%**; **FR-I07 within tolerance across full corpus**; collision gate green; C1 verified; perf budgets met; Scenarios A–F on **Windows AND macOS**; security sign-off; **human perceptual sign-off**; docs complete. (PRD §12, §11 Phase 3.)

### 3.1 Automation + API — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-3001|`(red-first)`|Automation server + CLI contract tests (Excel & JSON template-merge; named placeholders; fixed chart width on category-count change) — RED.|SDET|T-B007|FR-E08|
|T-3002|🧱|**Automation CLI + server** (headless core; gRPC/REST `CONFIRM`); reuses core, no host app.|Automation Eng|T-3001,T-0021|FR-E08, H06|
|T-3003|`[P]`|**Public API** (batch ops + style-file introspection).|Automation Eng|T-3002|FR-H06|

### 3.2 Services — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-3010|`[P]`|**Chart scanner** (image/PDF→datasheet; robust low-contrast; CV/vision-model). Separable.|Chart-Scanner Eng|T-0012|FR-E09|
|T-3011|`[P]`|**Tableau connector** (dashboard view→chart; store binding; re-update).|Connectors Eng|T-1041|FR-E07|

### 3.3 Library (Epic G) — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-3020|`[P]` 🔒|**Asset search** (local/network/OneDrive; filename + on-slide text; filters; **permission-preserving, no upload**).|Connectors Eng|T-2042|FR-G01|
|T-3021|`[P]`|**250+ adaptive templates** (auto-adapt to theme/master) + insert modes (source/dest/objects-only).|Style Eng|T-2040|FR-G02, G04|
|T-3022|`[P]`|**Icons + stock images** providers; **SVG→editable shapes**; **bitmap→managed image** (layout-participating; fixed dims).|Style Eng|T-3021|FR-G03, G05, G06|
|T-3023|`[P]`|**Duplicate-result filter** + in-product library training.|Tech Writer|T-3020|FR-G07|

### 3.4 Platform completion + deployment — `[P]`

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-3030|🧱|**macOS adapter to full parity** (native, or Office.js fallback per CL-1/R-2 ADR). Scenarios A–F on Mac.|macOS-Adapter Eng|T-0031,T-1044|FR-H03, R-2|
|T-3031|`[P]`|**Co-authoring / SharePoint** support.|OOXML Eng|T-2052|FR-H08|
|T-3032|`[P]`|**Silent/admin deployment** (ribbon customization; enforced style files; admin links; product tab; proxy support).|Windows-Adapter Eng|T-2044|FR-H05, NFR-08|
|T-3033|`[P]`|**Offline manual + in-product tips/training** + release notes.|Tech Writer|T-2053|FR-H07|
|T-3034|`[P]`|**Licensing/activation module** (pluggable slot; 🔴 CL-9 commercial model needed for final shape).|Automation Eng|T-3002|FR-H09, CL-9|

### 3.5 Epic J — AI Assist (conditional)

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-3040|🔴|**Epic J gate check.** Build only if CL-B includes think-cell 15/Assist. Current TRD §1 decision = **No** → tasks deferred to post-parity backlog. If reactivated: FR-J01–J04 + AI provider + on-prem/API decision (C2).|Product Owner|T-B001|Epic J, CL-B, NFR-04|

### 3.6 Final parity gate

|ID|`[P]`|Task|Owner|Depends|Traces|
|--|--|--|--|--|--|
|T-3050||**Drive parity ledger to 100%** (FR-I06); close every red/partial row with evidence.|Product Owner|all Phase-3|FR-I06|
|T-3051|🧱|**Equivalence harness across full corpus within tolerance** (FR-I07).|QA / Verification Lead|T-1052,T-3050|FR-I07|
|T-3052||**Perf budgets met** (NFR-01) — numbers set in `implementation-details/perf-budgets.md`.|Performance Eng|T-3002|NFR-01|
|T-3053|🔴 🔒|**GATE 3 review + release.** Evidence: I06=100%; I07 in tolerance; collision gate green; C1 verified; perf met; A–F on Win **and** Mac; security/privacy sign-off; **human perceptual sign-off**; docs complete. → **Parity declared. Ship.**|Orchestrator + all gatekeepers + human|T-3030,T-3050,T-3051,T-3052|PRD §12, Part D Gate 3|

-----

## Detail-file generation tasks (TRD §16.2 — write alongside the contracts they detail)

|ID|`[P]`|Task|Owner|Traces|
|--|--|--|--|--|
|T-D001|`[P]`|`implementation-details/layout-solver.md` — full solver spec (slots, scoring, incremental re-solve, leader-line routing).|Layout-Solver Eng|FR-B01|
|T-D002|`[P]`|`implementation-details/data-model.md` — full entity schemas (TRD §7).|Principal Architect|TRD §7|
|T-D003|`[P]`|`implementation-details/traceability.md` — full component→FR map.|Product Owner|TRD §4|
|T-D004|`[P]`|`implementation-details/perf-budgets.md` — numeric NFR-01 bounds (set post-solver baseline, TQ-3).|Performance Eng|NFR-01, TQ-3|
|T-D005|`[P]`|`research.md` — R-2 + solver-technique spike findings.|Architect|R-2, TQ-1/TQ-2|

-----

## Critical-path summary (the spine)

```
T-B005/006 (contracts) → T-B010 (red tests) → T-B012 (oracle) 
   → T-0015/0016 (SOLVER 🧱) → T-0023 (native shapes, real PPT) → GATE 0 🔴(CL-1)
   → [R-6 cleared] → T-1020 (solver generalized) + T-1041 (Excel links) → T-1050 (A–D) → GATE 1
   → T-2011 (fiscal) + T-2051 (cross-Office) + T-2052 (round-trip) → GATE 2
   → T-3030 (Mac parity) + T-3050/3051 (ledger 100% + equivalence) → GATE 3 🔴 → SHIP
```

Everything else hangs off this spine as `[P]` work within its phase. **No gate is waived for schedule; nothing is "done" without ledger evidence.**
