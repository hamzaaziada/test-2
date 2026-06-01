# Phase-0 Fixture Set

The fixtures encoded in `tests/test_e2e_gate.cpp` form the collision-gate
integration suite (FR-I03). Each fixture is a `Datasheet` driven through the
full pipeline: `buildStackedColumn` or `buildWaterfall` → `deriveLabelRequests`
→ `placeLabels` → `scoreLabels`. The gate passes only when every fixture
produces `report.pass == true` (zero overlapping label pairs, zero off-canvas
labels).

## Fixture Map

| ID | Name | Builder | PRD §5.2 edge case |
|----|------|---------|-------------------|
| (a) | `normal_3x4` | `buildStackedColumn` | Baseline: 3 series × 4 categories. Verifies standard interior label placement with no pathological geometry. All labels fit comfortably inside their respective segments. |
| (b) | `tiny_segment` | `buildStackedColumn` | **Tiny-segment → leader-line**: one series has values of 2.0 against dominant values of 80–90. The resulting segment is only a few pixels tall; the interior label candidate cannot be placed without colliding with neighbouring labels. The solver must fall back to an external slot and emit a leader line (FR-B02 / PRD §5.2 tiny-segment edge case). |
| (c) | `dense_8x4` | `buildStackedColumn` | **Dense chart**: 8 categories × 4 series yields 32+ segments plus 8 total labels. Column widths shrink to ~100 px each, stressing the alignment and external-fallback logic. Exercises the solver's O(L·C·O) complexity budget (PRD §5.2 density edge case). |
| (d) | `waterfall` | `buildWaterfall` | **Waterfall chart**: single-series running-total bars with mixed positive and negative deltas (PRD §5.2 waterfall edge case). Bar geometry differs fundamentally from stacked columns; verifies `buildWaterfall` geometry and that the oracle gate holds across chart types (FR-A04). |

## Canvas

All fixtures run on a 960 × 540 canvas (16:9 slide coordinates), matching the
reference slide size used throughout Phase-0.

## External Slot Column

`deriveLabelRequests` generates external candidates stacked in a vertical
column 10 px to the right of the plot area (x = `plot.right() + 10`), spaced
18 px apart — strictly greater than the 14 px label height — so external slots
are mutually non-overlapping by construction. This ensures the solver always
has a collision-free fallback for every label regardless of how dense the
interior becomes.
