// Lattice verification — Layout-quality oracle contract (FR-I01) + collision gate (FR-I03).
// Implemented by QA / Verification Lead (T-B012). The keystone automated "is this right?"
// signal: overlap MUST be 0 for a layout to pass (Master Directive 2/3).
#pragma once
#include <string>
#include <vector>

#include "lattice/geometry.hpp"

namespace lattice {

struct OracleReport {
  double totalOverlapArea = 0.0;  // summed positive overlap among all label boxes
  int overlappingPairs = 0;       // count of label-box pairs that overlap
  int offCanvasLabels = 0;        // labels not fully inside the canvas
  double alignmentScore = 1.0;    // 1.0 = perfectly aligned columns of labels (higher better)
  bool pass = false;              // true iff overlappingPairs == 0 && offCanvasLabels == 0

  std::string summary() const;
};

// Score a set of placed label boxes against the canvas. The hard gate (FR-I03) is
// overlappingPairs == 0 && offCanvasLabels == 0.
OracleReport scoreLabels(const std::vector<Rect>& labelBoxes, const Rect& canvas);

}  // namespace lattice
