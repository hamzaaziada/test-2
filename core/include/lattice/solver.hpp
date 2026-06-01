// Lattice core — Layout constraint solver contract (THE MOAT: FR-B01/B02/C12).
// Implemented by Layout-Solver Engineer (T-0016). MUST be deterministic (same input ->
// same output, for FR-I07 equivalence) and collision-free. Leader lines route to external
// slots when interior space is insufficient (tiny-segment edge case, PRD §5.2).
#pragma once
#include <optional>
#include <utility>
#include <vector>

#include "lattice/geometry.hpp"

namespace lattice {

// A single label that must be placed somewhere among its candidate slots.
struct LabelRequest {
  int id = 0;
  Rect anchor{};                  // the datapoint the label belongs to (e.g. segment center box)
  std::vector<Rect> candidates;   // ranked placement options; candidates[0] = most preferred
  bool allowExternal = true;      // may overflow to an external slot + leader line
  std::vector<Rect> externalCandidates;  // used only when no interior candidate is collision-free
};

// The solver's chosen placement for one label.
struct Placement {
  int id = 0;
  Rect box{};                                       // chosen rectangle
  bool external = false;                            // true if placed in an external slot
  std::optional<std::pair<Point, Point>> leader;    // leader line (anchor -> label) when external
};

// Place every label collision-free against (a) each other and (b) the obstacles, staying
// within `canvas`. Deterministic. If a label cannot be placed without overlap interiorly and
// allowExternal is set, an external candidate + leader line is used.
std::vector<Placement> placeLabels(const std::vector<LabelRequest>& labels,
                                   const std::vector<Rect>& obstacles, const Rect& canvas);

}  // namespace lattice
