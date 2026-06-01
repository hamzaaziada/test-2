// Lattice core — Data & Datasheet engine contract (FR-A02/A03/E01).
// Implemented by Data/Datasheet Engineer (T-0012). Deterministic, no I/O.
#pragma once
#include <string>
#include <vector>

namespace lattice {

struct Series {
  std::string name;
  std::vector<double> values;  // one value per category, aligned to Datasheet::categories
  std::string color = "#4472c4";
};

// A minimal stacked-chart datasheet: categories on the x-axis, series stacked within each.
struct Datasheet {
  std::vector<std::string> categories;
  std::vector<Series> series;

  // Number of category columns (== categories.size()).
  std::size_t categoryCount() const;

  // Sum of all series values within category i (FR-A03 automatic totals).
  double columnTotal(std::size_t categoryIndex) const;

  // Largest column total — used to scale the value axis.
  double maxColumnTotal() const;

  // Sum across every series and category (FR-A03 grand total).
  double grandTotal() const;
};

}  // namespace lattice
