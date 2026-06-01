# Product Requirements Document — “Lattice” (Full-Parity Edition)

> **Working name:** Lattice *(placeholder — rename freely)*
> **One-line:** A PowerPoint + Excel add-in that achieves **full functional parity with think-cell**, automating consulting-grade charts, annotations, and slide layout while eliminating manual positioning.
> **Document type:** Product/Feature Specification (Spec Kit `spec.md` convention — WHAT & WHY only; HOW is deferred to the companion TRD/`plan.md`)
> **Status:** Draft for review — 4 open `[NEEDS CLARIFICATION]` markers (§13) must close before TRD generation
> **Created:** 2026-06-01
> **Version:** 2.0 — **Full Parity** (supersedes the phased-MVP draft)

-----

## 0. How to read this document

This PRD follows Spec Kit discipline and a single overriding instruction: **the target is full parity with think-cell, not a reduced subset.**

- **Every functional requirement below is REQUIRED for parity-complete.** Nothing is optional. The **`Seq`** tag (1 / 2 / 3) denotes *build order only* — it never implies a feature can be dropped.
- WHAT/WHY only. Architecture, algorithms, and stack are marked **`→ TRD`** and deferred.
- Genuine ambiguities use **`[NEEDS CLARIFICATION: …]`**. Choosing full parity *resolves most* of the prior draft’s open questions by fiat (Mac, data limits, connectors, automation inputs, etc.) — only 4 genuine decisions remain (§13).
- Acceptance criteria are testable so they can generate implementation **and** tests.
- **The operational definition of “parity” is in §1.4 and §12** (a pinned benchmark + an output/behavioral-equivalence harness). Without those, “full parity” against a continuously-shipping incumbent is mathematically undefined — pinning the baseline is spec hygiene, not scope reduction.

-----

## 1. Problem, motivation & parity definition

### 1.1 The problem

Analysts who produce data-dense decks (consulting, banking, corporate strategy/finance, PE) lose disproportionate time to the *mechanics* of slide-making: positioning labels so they don’t collide, recomputing totals, aligning shapes, rebuilding charts when numbers change, keeping agendas in sync. Native PowerPoint offers chart primitives but no automation of layout, labeling, annotation, or live data-linkage at the fidelity this audience needs.

### 1.2 Why parity, and why it’s hard

think-cell is the category leader; matching it removes the incumbent’s only real objection (“but it doesn’t do *X* that think-cell does”). The defensible core being matched is **not the chart catalogue** (PowerPoint ships waterfalls and Marimekkos) — it is **(a)** the automatic label-placement / layout-constraint engine, **(b)** the breadth spanning charts + Gantt + agendas + process flows + data-linked reporting + asset library, and **(c)** the annotation vocabulary analysts argue with. Parity therefore means matching the *hard* parts, not just drawing the same chart shapes.

### 1.3 North-star outcomes (unchanged by parity scope)

- Reproduce a reference consulting chart in **<60 s with zero manual label dragging**.
- **Zero label overlaps** on the regression corpus.
- A recipient **without** Lattice still sees a correct, editable slide.

### 1.4 Parity baseline definition (REQUIRED reading)

“Full parity” is defined as **functional and output equivalence to a pinned snapshot of think-cell**, because the incumbent ships continuously and an unpinned target is unbounded.

- **Parity baseline:** the think-cell **Suite** (Core + Charts + Library) at version **`[NEEDS CLARIFICATION: CL-A — exact version + snapshot date, e.g., "14.x as of 2026-06-01"]`**.
- **think-cell 15 / Assist (AI):** currently early-access; included in parity scope **only if CL-B says so** (see Epic J).
- **Post-baseline incumbent releases:** tracked in a separate, explicitly-labelled backlog — *not* part of the v1 parity definition. Maintaining parity over time is an ongoing program, acknowledged but out of this PRD’s “done” criteria.
- **“Equivalent”** is operationalized by the equivalence harness in FR-I07 (visual + behavioral equivalence within a defined tolerance), measured by the parity checklist FR-I06.

-----

## 2. Goals & non-goals

### 2.1 Goals

G1 Eliminate manual positioning of labels, totals, and smart elements · G2 Keep charts/tables **live-linked** to data · G3 Match the full **annotation vocabulary** · G4 Automatic **brand compliance** · G5 **Graceful degradation** for non-users · G6 Match think-cell’s **breadth** (Gantt, agendas, process flows, automation, library) · G7 Build the **verification machinery** required to *prove* parity.

### 2.2 Non-goals

- NG1 — A general-purpose BI/dashboarding tool (different product).
- NG2 — Replacing Excel or PowerPoint; Lattice augments both.
- NG3 — **Non-Office platforms** (Google Slides, Keynote). The parity benchmark is Office-only, so these are out of scope. *(Resolves the prior CL-11.)*
- NG4 — Parity with think-cell features released **after** the pinned baseline (§1.4).

> *Removed from the prior draft:* “not bit-for-bit reproduction” and “not a native rewrite” are **no longer non-goals** — full parity may require both. See §4.2.

-----

## 3. Target users & personas

|Persona                                  |Context                                   |Primary need                                        |Success signal                                            |
|-----------------------------------------|------------------------------------------|----------------------------------------------------|----------------------------------------------------------|
|**Priya — Strategy Consultant (primary)**|Builds 40+ data slides/week under deadline|Speed to polished, accurate charts; zero fiddling   |<1 min/chart; trusts it on screen with a client           |
|**Omar — IB / PE Analyst**               |Models in Excel; presents to IC           |Live Excel→slide; waterfalls, football fields       |Updates model, refreshes deck in one click                |
|**Lena — Corporate Strategy/Finance**    |Recurring monthly/quarterly reporting     |Report automation; brand compliance                 |Regenerates a 30-slide report from new data, no rework    |
|**Sam — Report-Automation Engineer**     |Headless pipelines                        |Programmatic generation (template + data)           |Produces slides from server/CLI without opening PowerPoint|
|**Dieter — IT / Deployment Admin**       |Org-wide rollout & brand control          |Silent deploy, enforced styles, data-leak prevention|Locked-down, on-brand install; no data egress             |

**Anti-persona:** casual users making occasional simple charts — native PowerPoint already serves them.

-----

## 4. Context & constraints

### 4.1 Environment

Users work **inside PowerPoint and Excel**, on Excel-originated data, under deadline, often with sensitive client/financial content, sometimes in non-English / RTL languages, on **both Windows and macOS** (parity requires both — see FR-H03).

### 4.2 Platform & delivery constraint — narrowed by the parity decision

Full parity requires: deep manipulation of slide/chart objects, **custom-quality rendering and label placement**, live Office object-model integration in both PowerPoint and Excel, and **output as native PowerPoint objects** for non-users (C1). A pure standalone web app cannot meet C1 + deep manipulation, and the Office.js API surface may be insufficient for 100% of these behaviors.

`[NEEDS CLARIFICATION: CL-1 — delivery model, now narrowed]` Parity strongly implies a **native add-in (Windows COM + a macOS-native equivalent)**, mirroring the benchmark. Confirm native, **or** commission a feasibility spike proving Office.js can hit 100% parity (especially collision-free label placement and native-object output). This single decision governs the entire TRD and must be resolved first. *(All architecture → TRD.)*

### 4.3 Hard product constraints

- **C1 — Graceful degradation (make-or-break):** finished output renders and stays editable as native PowerPoint objects for recipients without Lattice.
- **C2 — Data stays local by default** (privacy is a buying criterion). `[NEEDS CLARIFICATION: CL-B — does the AI tier (Epic J) require egress, and is the parity baseline inclusive of it?]`
- **C3 — Localization incl. RTL** (Arabic, Hebrew) for charts and layout.
- **C4 — Match-or-beat the benchmark’s perceived performance and supported data limits** (e.g., >4,000 scatter points). *(Resolves the prior CL-3/CL-10 as “≥ benchmark.”)*

-----

## 5. User scenarios (acceptance-level)

### 5.1 Primary story

> As an analyst, I sketch a chart, type raw numbers, and get a clean, correctly-labeled, on-brand chart with totals — without positioning anything by hand — in under a minute.

**A — Scribble to chart:** *Given* a blank slide, *when* I insert a stacked column chart and type raw values (no totals/rounding), *then* the chart updates live, totals are computed & placed, all labels are placed without overlap, and label color auto-contrasts with its fill.

**B — Data changes:** *Given* a chart linked to Excel, *when* the source changes and I refresh, *then* chart, labels, totals, and CAGR/difference arrows update with no manual repositioning and the layout rebalances.

**C — Recipient without the add-in:** *Given* I send a finished deck, *when* it opens in stock PowerPoint, *then* every element renders correctly and is editable as native objects.

**D — Annotated growth story:** *Given* a multi-year column chart, *when* I add a CAGR arrow and a difference arrow, *then* values compute correctly, render with expected suffix (e.g., `p.a.`), and reposition automatically on change.

**E — Recurring report:** *Given* a template + new data, *when* I run automation (interactive or headless), *then* a fully formatted on-brand deck is produced with charts/tables/text populated, no rework.

**F — Project timeline:** *Given* project dates in Excel (on a fiscal calendar), *when* I build a Gantt chart and link the dates, *then* activities, milestones, brackets, and dependency lines render on the correct date scale and update when dates change.

### 5.2 Edge cases (the long tail — where parity is won or lost; all become regression fixtures, FR-I02)

Tiny segments → leader-line external labels (still collision-free) · negative & mixed waterfalls; subtotal/total columns holding position under sort · dual / reversed / ordinal / percentage axes; axis breaks; same-scale across differing ranges · dense charts (30+ categories, many series; >4,000 scatter points) · rotated / multi-line / very long labels · RTL flip of an existing chart · datasheet ≠ used range; pasted header rows misread as totals · moved/renamed Excel source; manually recalculated sheets · fiscal-year leap weeks (4-4-5 / 4-5-4 / 5-4-4) · “Other”-series aggregation boundaries · conditional-formatting color scales imported from Excel.

-----

## 6. Functional requirements — full think-cell surface

> **All requirements are REQUIRED for parity.** `Seq` = build order only. All HOW → TRD.

### Epic A — Chart types & core charting `Seq 1`

- **FR-A01** Insert any chart type from a menu/ribbon, placed like a native shape with snapping/alignment to existing objects; auto-selected after insert.
- **FR-A02** Embedded **Excel-like datasheet**; **live** chart update on edit; chart auto-grows/shrinks with the used range; tab/enter navigation; paste from Excel.
- **FR-A03** Automatic **totals and grand totals**; user enters only raw values (no manual rounding/summing).
- **FR-A04 Chart catalogue (all required):** stacked column, clustered column, stacked bar, clustered bar, line, area, stacked area, **100%** (column/area), **waterfall** (with category-sort options that keep sum/subtotal columns in place), **Mekko/Marimekko** (with percentage axis + grand total), **pie**, **doughnut** (with sort order), **scatter**, **bubble**, **combination/combo**, **profile/rotated line**, rotated stacked/clustered, rotated combo.
- **FR-A05** **Convert** between chart types post-creation (e.g., stacked↔clustered, segments↔line); toggle **absolute↔percentage** even without a visible axis.
- **FR-A06** Select features individually / in groups, incl. **progressive “select-wider”** via a single repeated shortcut; restyle via a contextual **mini-toolbar**.
- **FR-A07** Direct-handle control of **column/bar width & gap**, **segment order**, **milestone shapes**, and **baseline** (removable; adjustable weight).

### Epic B — Automatic layout & smart elements (the moat) `Seq 1`

- **FR-B01** **Collision-free automatic label placement** for every chart label, with overflow to **external labels + leader lines** when interior space is insufficient. *Highest-value, highest-difficulty requirement.* `→ TRD` (constraint solver).
- **FR-B02** Continuously **re-balancing slide layout**: insert/resize/remove re-flows related smart elements while honoring user locks.
- **FR-B03** **Smart text boxes** that participate in layout (push/resize neighbors as text changes).
- **FR-B04** **Process flows** (chevron/pentagon) from building blocks; single-click duplication; horizontal↔vertical direction toggle re-flows the whole structure.
- **FR-B05** **Agendas**: multi-level TOC that stays in sync as chapters are added/reordered; visual styles incl. **flat**; **hide chapters of a level**; brand-styled; **default agenda layouts**.
- **FR-B06** **Tables** with associated datasheet; auto formatting/sizing; stripes; cell styles.
- **FR-B07** **Harvey balls & checkboxes**, optionally **data-linked**, groupable into tables/elements, with custom images (e.g., traffic lights) and non-square shapes.
- **FR-B08** **Rounded rectangles** as managed elements.
- **FR-B09** Layout tools: **align, match-size, swap positions, scale-together** (font/shape proportions preserved), **group** (incl. mixing Lattice elements with native shapes), rotate/flip, **paste text into multiple objects**, arrange, resize, select-objects.
- **FR-B10** Element **lock model** (fix size/position to override automation; best-of-both-worlds with native behavior); option to lock by default.
- **FR-B11** **Preset slide layouts / chart placeholders** defined in the style file; **slide-position thumbnails** shown on insert/move.
- **FR-B12** **Same-scale linking** across multiple charts (persistent; survives axis breaks; mix-and-match segment/axis/gridline/datapoint as the link anchor; reset to independent).

### Epic C — Annotations & chart features `Seq 1–2`

- **FR-C01** **CAGR arrows** (auto-computed; editable `p.a.` suffix).
- **FR-C02** **Difference arrows** (absolute and/or %).
- **FR-C03** **Value lines** (auto **mean or median**).
- **FR-C04** **Net lines** (category-specific benchmark/target overlay).
- **FR-C05** Labels: series, category, segment, **grand-total**; **number format by example**; magnitude/units; rotation; **persistent font color**; up to 255-char labels; selective 4-digit year on axis labels.
- **FR-C06** Axes: value, category, **ordinal**, percentage; **axis breaks**; reversed axis; gridlines-only mode; category sort by total.
- **FR-C07** **Error bars** (enabling **football-field** charts).
- **FR-C08** **Trendlines**: linear, **polynomial (quadratic/cubic/quartic)**, log, exponential, power; **partitions** with fills (incl. regions bounded by non-linear trendlines).
- **FR-C09** **Legends**: order by datasheet / visual / alphabetical; highlight-series-on-select.
- **FR-C10** **Tooltips** showing labels + numeric values per datapoint.
- **FR-C11** **“Other” series** aggregation via drag handle; datasheet highlight of designated series; toggle inclusion.
- **FR-C12** **All annotations reposition automatically** on data/geometry change (ties to FR-B01).

### Epic D — Gantt / timelines `Seq 2`

- **FR-D01** Timeline with **activities** (bars), **milestones** (multiple shapes), **brackets**, and **processes**.
- **FR-D02** **Date scale** from day to year; customizable appearance (lines, fills, scale look) via style file.
- **FR-D03** **Link / dependency lines** between activities.
- **FR-D04** **Excel-linked dates**: activities & milestones update when Excel dates change.
- **FR-D05** **Fiscal calendars**: custom fiscal-year end; **4-4-5 / 4-5-4 / 5-4-4** conventions; **leap weeks**; weekday-aligned year ends (e.g., “Friday nearest Jan 31”).
- **FR-D06** Move all date items at once; full **RTL** Gantt support.

### Epic E — Data layer & automation `Seq 1–3`

- **FR-E01** Internal datasheet behaves like a lightweight spreadsheet.
- **FR-E02** **Live Excel data links**: link a range; one-click refresh; optional auto-update; a **link-management dialog** that lists/locates/repairs links, highlights linked elements on the slide, updates/reverts, selects-all-by-source, deletes disconnected links, and **alerts on source change with alternate-source selection**.
- **FR-E03** **Editable data layout** (toggle which row/column is series vs. labels vs. totals) so charts link to existing sheets without restructuring them.
- **FR-E04** **Slide workbook**: an embedded spreadsheet feeding all data-driven elements on a slide.
- **FR-E05** **Linked data tables** (a table linked to Excel, auto-updating).
- **FR-E06** **Text fields linked to Excel cells**, embeddable in any label/text box, auto-updating.
- **FR-E07** **Tableau connector** (dashboard view → chart; re-update from the source). *(Resolves prior CL-5.)*
- **FR-E08** **Report automation**, **both Excel-driven and JSON-driven** *(resolves prior CL-4)*: template-merge; **named text-field placeholders**; **headless / CLI / server** modes; fixed chart width when category count changes.
- **FR-E09** **Chart scanner**: extract data from a chart **image/PDF** into a datasheet; robust on low-contrast images. `→ TRD` (CV).
- **FR-E10** **Excel tools**: CAGR calculation; table & cell styles; data rounding.
- **FR-E11** **Conditional-formatting import**: color scales, cell-reference rules, text/date-based rules reflected automatically in charts and tables.

### Epic F — Brand compliance & slide hygiene `Seq 2`

- **FR-F01** **Style files**: default colors/fonts/color-schemes; custom line colors/styles; arrow fills/outlines; Harvey-ball/checkbox styles (incl. images); **Gantt styles**; **fiscal-calendar** definitions; **agenda layouts**; **preset chart layouts**; legend-label fonts; default-displayed labels; theme shades as base+brightness; “scheme-not-remembered” option.
- **FR-F02** Default theme; **Load Style File** with recent-files list.
- **FR-F03** **Clean up / sanitize**: remove comments, speaker notes, unused masters/layouts, animations, and data links/slide workbooks; **randomize all numbers** — before sharing.
- **FR-F04** **Send / Save slides**: export selected slides or whole deck; via email incl. **Gmail**; choose included slides; append date/time to filename.
- **FR-F05** Productivity tools: **switch decimal symbol**, **replace/resize fonts** (Latin/Asian/complex scripts), **set proofing language**, **insert symbols**, **remove animations** — across selection/slide/deck.
- **FR-F06** **Display file names/paths on slides**.
- **FR-F07** **Color picker** with live preview, hex/RGB input, and **eyedropper**.
- **FR-F08** **Share with non-users** via native objects (implements C1).

### Epic G — Library `Seq 3`

- **FR-G01** **Asset search** across local / network / OneDrive by filename and on-slide text; filters (date, aspect ratio, author, master, layout); **permission-preserving; no upload of user files**.
- **FR-G02** **250+ slide templates** that auto-adapt to the active theme/master.
- **FR-G03** **Icons** (Magnific/Freepik) + **stock images** (Pexels/Unsplash free; Getty/Brandfolder/Canto with subscription). *(Commercial terms → business.)*
- **FR-G04** Insert with **source formatting** / **destination formatting** / **objects-only**.
- **FR-G05** Convert **SVG icons → editable shapes**.
- **FR-G06** Convert bitmap images → managed images (participate in layout); **fixed image dimensions**; insert stock photos into the auto-layout.
- **FR-G07** Duplicate-result filter; in-product training for the library.

### Epic H — Platform, compatibility, deployment & API `Seq 1 (infra) → 3`

- **FR-H01** Operate in-app in PowerPoint **and** Excel (per CL-1).
- **FR-H02** **Graceful degradation** (C1) verified by tests (FR-I05).
- **FR-H03** **Windows + macOS parity**; Windows on **ARM**; 32/64-bit. *(Resolves prior CL-8: Mac is in v1.)*
- **FR-H04** **RTL** (Arabic/Hebrew) with automatic flipping on language change.
- **FR-H05** **Silent/admin deployment**; ribbon customization; **enforced default style files**; admin resource links; a dedicated product tab; proxy support.
- **FR-H06** **Public API** for programmatic control (underpins FR-E08; incl. batch operations and style-file introspection).
- **FR-H07** **Offline manual**; in-product tips/training.
- **FR-H08** **Co-authoring / SharePoint** support.
- **FR-H09** Licensing & activation. `[NEEDS CLARIFICATION: CL-9 — commercial model; orthogonal to functional parity, does not block the build.]`

### Epic I — Quality, verification & parity proof (the unlock) `Seq 0–continuous`

> *Parity cannot be claimed without measuring it. Agents excel where an automatic “is this right?” signal exists; visual layout quality has none by default. These are first-class requirements.*

- **FR-I01** **Layout-quality oracle**: automated scoring of overlap (**target 0**), legibility, alignment, balance — run in CI on every change. `→ TRD`.
- **FR-I02** **Regression corpus**: curated, growing real-world decks/datasheets + every §5.2 adversarial case; each fixed bug adds a case.
- **FR-I03** **Collision metric hard gate**: overlap rate on the corpus must be 0; geometry regressions block release.
- **FR-I04** **Cross-Office test farm**: automated runs across supported Office versions / bitness / OS (the failure mode living in undocumented Office behavior). `→ TRD`.
- **FR-I05** **Round-trip + non-user-open fidelity test**: open→save→reopen, and open in a Lattice-free PowerPoint, must preserve appearance and editability (validates C1).
- **FR-I06** **Parity checklist** — the primary acceptance instrument: a maintained, versioned, feature-by-feature comparison against the **pinned baseline** (§1.4), each item tied to acceptance tests. **“Done” = 100% pass.**
- **FR-I07** **Output / behavioral equivalence harness** — given identical inputs, Lattice’s output is **visually and behaviorally equivalent to the benchmark within a defined tolerance** across the corpus. This is the operational definition of parity.

### Epic J — AI Assist (parity-baseline-dependent) `Seq 3`

> Required **only if** the parity baseline includes think-cell 15 / Assist (CL-B). If included, C2 privacy implications and provider choice must be resolved.

- **FR-J01** In-place **translation** (text box / slide / deck; layout preserved; 100+ languages).
- **FR-J02** **Data → chart** from research sources, generated as a real editable chart.
- **FR-J03** **Text refinement** (shorten/rephrase/bulletize), **auto action-titles**, **executive-summary** generation.
- **FR-J04** **In-app GenAI chat** for storyline/brainstorm and natural-language edits across slides.

-----

## 7. Non-functional requirements

- **NFR-01 Performance (≥ benchmark, C4):** chart insertion feels instant; label re-layout and datasheet→chart updates are snappy at the supported data scale. `→ TRD` for exact bounds.
- **NFR-02 Reliability:** no crashes / resource leaks in long sessions; never corrupt the host document. `→ TRD`.
- **NFR-03 Compatibility:** supported Office versions/bitness/OS explicitly defined and continuously tested (FR-I04).
- **NFR-04 Privacy/Security (C2):** files local by default; auditable network calls; data-leak prevention (FR-F03).
- **NFR-05 Accessibility:** keyboard-operable; output respects contrast; screen-reader-sane where the host allows.
- **NFR-06 Localization:** UI + number/date formatting localizable; RTL (FR-H04).
- **NFR-07 Observability:** diagnostic logging + reproducible-state “request support” path.
- **NFR-08 Installability:** low-friction individual install + enterprise silent deploy (FR-H05).

-----

## 8. Key entities (conceptual; schema → TRD)

**Element** (Chart, Table, TextBox, ProcessFlow, Agenda, Gantt, HarveyBall/Checkbox, RoundedRectangle, Image — geometry, locks, style binding) · **Chart** (Series, Categories, Datasheet, Axes, Labels, Annotations) · **Gantt** (Activities, Milestones, Brackets, Processes, DateScale, FiscalCalendar, DependencyLinks) · **Datasheet** (internal / Slide Workbook / External Link) · **External Link** (binding to Excel range / Tableau view / JSON; state: connected/stale/broken; update policy) · **Annotation** (CAGR/difference arrow, value/net line, total label — derived from data + geometry) · **Style File / Theme** · **Layout Constraint** (alignment, locks, ordering) · **Template** (named placeholders bound to incoming data).

-----

## 9. Dependencies & assumptions

**Assumptions:** A1 licensed, current Office (Win + Mac) · A2 data lives in Excel · A3 segment values privacy & brand control · A4 build is AI-agent-driven under Spec Kit (this PRD → TRD → tasks), favoring library-first, contract/test-first decomposition `→ TRD/constitution`.
**External dependencies:** D1 the Office extensibility surface for the chosen delivery model (CL-1) — **the gating dependency** · D2 AI provider/data partners for Epic J (CL-B) · D3 icon/stock-image providers for Epic G.

-----

## 10. Out of scope (explicit)

Non-Office platforms (NG3) · general BI dashboards (NG1) · replacing Office (NG2) · incumbent features released after the pinned baseline (NG4) · co-authoring beyond host-app capability · any “might need” feature not traceable to a persona need.

-----

## 11. Path to full parity (sequencing — no features dropped)

> Phases sequence the build; **the end state is 100% of the parity checklist (FR-I06), not a reduced product.** Each phase has an exit gate.

**Phase 0 — Feasibility spike & moat proof.** Build the hardest thing first on the candidate platform: collision-free label placement for stacked column + waterfall, with datasheet, auto-totals, one CAGR arrow, **and the FR-I01 oracle**. **Gate:** zero overlaps on a ~50-case set; **CL-1 decided on evidence.**

**Phase 1 — Charting + layout core.** Epic A, Epic B, Epic C (C01–C06, C12), Epic E links (E01–E03), graceful degradation (H02), verification gates (I01–I03, I07 v1). **Gate:** Scenarios A–D pass; <60 s reference chart; overlap = 0.

**Phase 2 — Breadth.** Remaining Epic C (C07–C11), **Epic D (Gantt)**, Epic E (E04–E06, E10–E11), Epic F (brand + hygiene), RTL (H04), cross-Office farm (I04), round-trip (I05), parity checklist stood up (I06). **Gate:** Scenarios E (interactive) & F pass; parity-checklist coverage measured and trending to 100%.

**Phase 3 — Full surface.** Automation incl. headless/API & JSON (E08, H06), chart scanner (E09), Tableau (E07), **Epic G (Library)**, co-authoring (H08), and **Epic J (AI) iff CL-B includes v15**. **Gate:** **FR-I06 = 100% pass; FR-I07 equivalence within tolerance across the corpus; C1 verified.** → *Parity declared.*

-----

## 12. Definition of Done (parity acceptance)

Parity is **complete** when **all** hold:

1. **FR-I06 parity checklist = 100%** against the pinned baseline (§1.4).
1. **FR-I07 equivalence harness**: visual + behavioral equivalence within tolerance across the full regression corpus.
1. **FR-I03 collision gate**: zero label overlaps on the corpus.
1. **FR-I05**: round-trip and non-user-open fidelity verified (C1).
1. All Scenarios A–F pass on Windows **and** macOS.

-----

## 13. Open questions — `[NEEDS CLARIFICATION]` (only 4 remain; the rest were resolved by choosing full parity)

**Resolved by the parity decision:** Mac in v1 (yes) · data/complexity limits (≥ benchmark) · automation inputs (Excel **and** JSON) · connectors (Tableau) · Google Slides (out) · performance (≥ benchmark) · inclusion of essentially all chart/feature areas (**all in**).

**Still genuinely open:**

|ID      |Question                                                                                                                                                                  |Blocks              |
|--------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------|
|**CL-A**|**Pin the parity baseline**: exact think-cell version + snapshot date. *(Without this, “full parity” is unbounded — this is the most important decision in the document.)*|§1.4, FR-I06, “Done”|
|**CL-1**|**Confirm native delivery** (Windows COM + macOS-native), **or** fund a spike proving Office.js can reach 100% parity (esp. FR-B01 + native-object output).               |Entire TRD          |
|**CL-B**|Does the parity baseline include **think-cell 15 / Assist (AI, Epic J)**? If yes, also decide AI provider + on-prem vs API given C2.                                      |Epic J, NFR-04      |
|**CL-9**|Commercial model (subscription / perpetual / freemium; seat vs usage). *Orthogonal to functional parity; needed for H09 but does not block the build.*                    |FR-H09              |

-----

## 14. Review & acceptance checklist (Spec Kit gate)

**Requirement quality** — [ ] No unresolved `[NEEDS CLARIFICATION]` *(4 remain — §13)* · [ ] Testable & unambiguous · [ ] Measurable success criteria (overlap rate, time-to-chart, % parity, equivalence tolerance) · [ ] No “might need” features · [ ] WHAT/WHY only (HOW → TRD).
**Coverage** — [ ] Scenarios A–F have acceptance criteria · [ ] Edge cases (§5.2) in the corpus (FR-I02) · [ ] NFRs specified · [ ] Graceful degradation (C1) tested (FR-I05) · [ ] **Full think-cell surface enumerated** (Epics A–J).
**Parity readiness** — [ ] Baseline pinned (CL-A) · [ ] Parity measured by checklist + equivalence harness (FR-I06/I07) · [ ] Hardest requirement (FR-B01) de-risked first (Phase 0) · [ ] Verification machinery (Epic I) in scope, not deferred.

-----

*End of PRD. Next Spec Kit step: resolve §13 (CL-A first), then generate the TRD/`plan.md` (architecture, data models, contracts) and `tasks.md` (parallelizable agent task list).*
