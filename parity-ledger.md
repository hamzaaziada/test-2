# parity-ledger.md — Lattice Parity Checklist (FR-I06)

> **The primary acceptance instrument** (PRD §1.4, §12; FR-I06). One row per functional requirement, each mapped to its acceptance test(s) and a status. **"Done" = 100% of rows at `parity`.**
> Maintained by the Product Owner; updated on every merge (Master Directive 1: no row moves to `green`/`parity` without evidence).

## Baseline header (🔴 CL-A — human checkpoint, set in T-B001)

| Field | Value |
|--|--|
| Parity baseline product | think-cell Suite (Core + Charts + Library) |
| Baseline version | **`[NEEDS CLARIFICATION: CL-A]`** — *provisional in TRD §1: 14.x GA* |
| Snapshot date | **`[NEEDS CLARIFICATION: CL-A]`** — *provisional: 2026-06-01* |
| v15 / Assist (Epic J) included? | **No** (TRD §1, CL-B) unless CL-B reopens |
| Equivalence tolerance | defined in `verification/equivalence/tolerance.config` (FR-I07) |

## Status legend

`not-started` → `red` (contract test exists, failing) → `green` (impl passes own tests) → `parity` (equivalence harness within tolerance vs. baseline + gate cleared).

## Ledger

| FR | Requirement (short) | Phase | Task(s) | Acceptance test | Status |
|--|--|--|--|--|--|
| A01 | Insert any chart, native placement, snapping | 1 | T-1010 | AT-A01 | not-started |
| A02 | Excel-like datasheet, live update, paste | 0 | T-0012 | AT-A02 | green (P0) |
| A03 | Automatic totals / grand totals | 0 | T-0012 | AT-A03 | green (P0) |
| A04 | Full chart catalogue | 0–1 | T-0014,T-1002..1008 | AT-A04 | green (P0: stacked col + waterfall only) |
| A05 | Convert chart types; abs↔% toggle | 1 | T-1004,T-1009 | AT-A05 | not-started |
| A06 | Select-individual/group; select-wider; mini-toolbar | 1 | T-1010 | AT-A06 | not-started |
| A07 | Direct-handle width/gap/order/milestone/baseline | 1 | T-0014,T-1010 | AT-A07 | not-started |
| B01 | Collision-free auto label placement + leader lines | 0 | T-0016,T-1020 | AT-B01 (oracle=0) | green (P0: oracle=0) |
| B02 | Re-balancing slide layout | 0–1 | T-0017,T-1020 | AT-B02 | not-started |
| B03 | Smart text boxes in layout | 1 | T-1021 | AT-B03 | not-started |
| B04 | Process flows + reflow | 2 | T-2030 | AT-B04 | not-started |
| B05 | Agendas (multi-level sync) | 2 | T-2031 | AT-B05 | not-started |
| B06 | Tables with datasheet | 2 | T-2032 | AT-B06 | not-started |
| B07 | Harvey balls & checkboxes | 2 | T-2033 | AT-B07 | not-started |
| B08 | Rounded rectangles (managed) | 2 | T-2034 | AT-B08 | not-started |
| B09 | Layout tools (align/match/swap/scale/group) | 1 | T-1023 | AT-B09 | not-started |
| B10 | Element lock model | 1 | T-1022 | AT-B10 | not-started |
| B11 | Preset layouts / placeholders / thumbnails | 1–2 | T-1043,T-2034 | AT-B11 | not-started |
| B12 | Same-scale linking across charts | 1 | T-1024 | AT-B12 | not-started |
| C01 | CAGR arrows | 0 | T-0018 | AT-C01 | green (P0) |
| C02 | Difference arrows (abs/%) | 1 | T-1031 | AT-C02 | not-started |
| C03 | Value lines (mean/median) | 1 | T-1032 | AT-C03 | not-started |
| C04 | Net lines | 1 | T-1033 | AT-C04 | not-started |
| C05 | Labels + number-format-by-example | 1 | T-1034 | AT-C05 | not-started |
| C06 | Axes (value/cat/ordinal/%/breaks/reversed) | 1 | T-1035 | AT-C06 | not-started |
| C07 | Error bars → football-field | 2 | T-2001 | AT-C07 | not-started |
| C08 | Trendlines + partitions | 2 | T-2002 | AT-C08 | not-started |
| C09 | Legends (order, highlight) | 2 | T-2003 | AT-C09 | not-started |
| C10 | Tooltips | 2 | T-2004 | AT-C10 | not-started |
| C11 | "Other" series aggregation | 2 | T-2005 | AT-C11 | not-started |
| C12 | All annotations auto-reposition | 1 | T-1036 | AT-C12 | not-started |
| D01 | Gantt activities/milestones/brackets/processes | 2 | T-2012 | AT-D01 | not-started |
| D02 | Date scale day→year, styled | 2 | T-2012 | AT-D02 | not-started |
| D03 | Dependency lines | 2 | T-2013 | AT-D03 | not-started |
| D04 | Excel-linked dates | 2 | T-2014 | AT-D04 | not-started |
| D05 | Fiscal calendars (4-4-5/4-5-4/5-4-4, leap) | 2 | T-2011 | AT-D05 | not-started |
| D06 | Move-all dates; RTL Gantt | 2 | T-2015 | AT-D06 | not-started |
| E01 | Internal datasheet | 0 | T-0012 | AT-E01 | green (P0) |
| E02 | Live Excel links + management dialog | 1 | T-1041 | AT-E02 | not-started |
| E03 | Editable data layout | 1 | T-1042 | AT-E03 | not-started |
| E04 | Slide workbook | 2 | T-2020 | AT-E04 | not-started |
| E05 | Linked data tables | 2 | T-2021 | AT-E05 | not-started |
| E06 | Text fields linked to Excel cells | 2 | T-2022 | AT-E06 | not-started |
| E07 | Tableau connector | 3 | T-3011 | AT-E07 | not-started |
| E08 | Report automation (Excel & JSON, headless) | 3 | T-3002 | AT-E08 | not-started |
| E09 | Chart scanner (image/PDF→datasheet) | 3 | T-3010 | AT-E09 | not-started |
| E10 | Excel tools (CAGR/styles/rounding) | 2 | T-2023 | AT-E10 | not-started |
| E11 | Conditional-formatting import | 2 | T-2024 | AT-E11 | not-started |
| F01 | Style files (full surface) | 1–2 | T-1043,T-2040 | AT-F01 | not-started |
| F02 | Default theme; load style file | 1 | T-1043 | AT-F02 | not-started |
| F03 | Clean up / sanitize / randomize | 2 | T-2041 | AT-F03 | not-started |
| F04 | Send/Save slides (incl. Gmail) | 2 | T-2042 | AT-F04 | not-started |
| F05 | Productivity tools | 2 | T-2043 | AT-F05 | not-started |
| F06 | File names/paths on slides | 2 | T-2044 | AT-F06 | not-started |
| F07 | Color picker + eyedropper | 2 | T-2044 | AT-F07 | not-started |
| F08 | Share with non-users (native objects) | 1 | T-1044 | AT-F08 | not-started |
| G01 | Asset search (no upload) | 3 | T-3020 | AT-G01 | not-started |
| G02 | 250+ adaptive templates | 3 | T-3021 | AT-G02 | not-started |
| G03 | Icons + stock images | 3 | T-3022 | AT-G03 | not-started |
| G04 | Insert modes (source/dest/objects) | 3 | T-3021 | AT-G04 | not-started |
| G05 | SVG→editable shapes | 3 | T-3022 | AT-G05 | not-started |
| G06 | Bitmap→managed images | 3 | T-3022 | AT-G06 | not-started |
| G07 | Duplicate filter + library training | 3 | T-3023 | AT-G07 | not-started |
| H01 | Operate in PowerPoint + Excel | 0 | T-0023 | AT-H01 | not-started |
| H02 | Graceful degradation (tested) | 1 | T-1044,T-2052 | AT-H02 | not-started |
| H03 | Windows + macOS parity (ARM, 32/64) | 3 | T-3030,T-2051 | AT-H03 | not-started |
| H04 | RTL auto-flip | 2 | T-2050 | AT-H04 | not-started |
| H05 | Silent/admin deployment | 3 | T-3032 | AT-H05 | not-started |
| H06 | Public API | 3 | T-3003 | AT-H06 | not-started |
| H07 | Offline manual + tips | 3 | T-3033 | AT-H07 | not-started |
| H08 | Co-authoring / SharePoint | 3 | T-3031 | AT-H08 | not-started |
| H09 | Licensing & activation (🔴 CL-9) | 3 | T-3034 | AT-H09 | not-started |
| I01 | Layout-quality oracle | −1/0 | T-B012 | self (CI gate) | green (live) |
| I02 | Regression corpus | −1 | T-B013 | self | green (P0 fixtures seeded) |
| I03 | Collision gate (overlap=0) | 0 | T-0030 | self (CI gate) | green (live gate) |
| I04 | Cross-Office grid | 2 | T-2051 | self | not-started |
| I05 | Round-trip + non-user-open fidelity | 2 | T-2052 | self | not-started |
| I06 | Parity checklist (this file) | 2 | T-2053 | self | not-started |
| I07 | Output/behavioral equivalence harness | 1/3 | T-1052,T-3051 | self (tolerance) | not-started |
| J01–J04 | AI Assist | post-parity | T-3040 | — | deferred (CL-B=No) |

## Roll-up

| Metric | Count | Notes |
|--|--|--|
| Total functional FRs (non-deferred) | 73 | Epic J (J01–J04) deferred per CL-B=No |
| `green (P0)` — impl passes own headless tests | 9 | A02, A03, A04*, B01, C01, E01, I01, I02, I03 |
| `parity` — equivalent vs. pinned baseline | 0 | **blocked on CL-A** (no baseline pinned) + native adapters (R-1/R-2) |
| Overall parity | **0%** | Phase-0 moat green; Gates 1–3 not started |

\* A04 green for stacked-column + waterfall only; remaining catalogue is Phase-1 (T-1002…1008).

**Phase-0 evidence (Gate-0 candidate):** `ctest` 6/6 pass; e2e collision gate (`test_e2e_gate`) reports `overlaps=0` on all 4 fixtures (normal_3x4, tiny_segment→leader, dense_8x4, waterfall); `lattice_demo` emits `lattice_reference.svg` with oracle PASS. **What is NOT proven** (honest gaps, Master Directive 6): native-object output / graceful degradation (C1, needs real Office — R-1/R-2), equivalence vs. think-cell (FR-I07, needs pinned baseline CL-A + R-6 counsel), cross-Office grid (FR-I04), and everything in Gates 1–3.

**Parity is declared only when every non-deferred row = `parity` (FR-I06 = 100%), FR-I07 is within tolerance across the full corpus, and Gate 3 is cleared.**
