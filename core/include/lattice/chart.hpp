// Lattice core — Chart geometry contract (Epic A: FR-A04 stacked column + waterfall).
// Implemented by Chart-Geometry Engineer (T-0014). Pure functions: (data, area) -> shapes.
#pragma once
#include <string>
#include <vector>

#include "lattice/datasheet.hpp"
#include "lattice/geometry.hpp"

namespace lattice {

// One drawn segment of a stacked column (or one bar of a waterfall), with the label it carries.
struct Segment {
  Rect rect;          // pixel/slide-space rectangle of the filled segment
  std::string fill;   // hex color
  std::string label;  // value text to be placed (e.g. "42")
  double value = 0.0; // the raw value this segment represents
};

// The computed geometry of a chart, prior to label placement.
// `segmentLabelAnchor[i]` is the natural anchor point for segments[i]'s label (its center).
// `totalLabels` are the grand-total labels sitting above each column (FR-A03).
struct ChartLayout {
  Rect plot;                          // the plotting area
  std::vector<Segment> segments;      // all stacked segments / waterfall bars
  std::vector<std::string> totalText; // total label text per category column
  std::vector<Rect> totalAnchor;      // anchor rect (above each column) for each total label
};

// Build stacked-column geometry that fits `area`. Auto-scales to the max column total.
ChartLayout buildStackedColumn(const Datasheet& data, const Rect& area);

// Build a single-series waterfall. `data.series[0].values` are deltas; sums/subtotals
// (where value sentinel kind applies) keep their position. Minimal Phase-0 version:
// connected running-total bars from a single delta series.
ChartLayout buildWaterfall(const Datasheet& data, const Rect& area);

}  // namespace lattice
