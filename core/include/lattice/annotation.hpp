// Lattice core — Annotation engine contract (Phase-0 slice: FR-C01 CAGR arrow).
// Implemented by Annotations Engineer (T-0018). Computes value + text; geometry/placement
// is handed to the solver (FR-C12).
#pragma once
#include <string>

namespace lattice {

struct CagrResult {
  bool valid = false;   // false if inputs are non-positive or periods <= 0
  double cagr = 0.0;    // compound annual growth rate, e.g. 0.123 == 12.3%
  std::string text;     // formatted label, e.g. "12.3% p.a." (FR-C01 editable suffix)
};

// CAGR between `first` and `last` over `periods` intervals (e.g. years between).
// suffix defaults to "p.a." per FR-C01.
CagrResult computeCagr(double first, double last, int periods, const std::string& suffix = "p.a.");

}  // namespace lattice
