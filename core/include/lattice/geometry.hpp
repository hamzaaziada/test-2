// Lattice core — 2D geometry primitives (header-only, pure, deterministic).
// Contract owned by Principal Architect (Phase −1, T-B005). DO NOT change signatures
// without a contract bump (Master Part C3); implementations depend on these shapes.
#pragma once
#include <algorithm>
#include <cmath>

namespace lattice {

struct Point {
  double x = 0.0;
  double y = 0.0;
};

// Axis-aligned rectangle, top-left origin, y grows downward (slide coordinates, EMU-agnostic).
struct Rect {
  double x = 0.0;
  double y = 0.0;
  double w = 0.0;
  double h = 0.0;

  constexpr double left() const { return x; }
  constexpr double top() const { return y; }
  constexpr double right() const { return x + w; }
  constexpr double bottom() const { return y + h; }
  constexpr double area() const { return w * h; }
  constexpr Point center() const { return Point{x + w / 2.0, y + h / 2.0}; }

  bool contains(const Rect& o) const {
    return o.left() >= left() && o.right() <= right() && o.top() >= top() && o.bottom() <= bottom();
  }
};

// Overlap area of two rectangles (0 if disjoint or merely touching).
inline double overlapArea(const Rect& a, const Rect& b) {
  const double dx = std::min(a.right(), b.right()) - std::max(a.left(), b.left());
  const double dy = std::min(a.bottom(), b.bottom()) - std::max(a.top(), b.top());
  if (dx <= 0.0 || dy <= 0.0) return 0.0;
  return dx * dy;
}

// True if the rectangles overlap with positive area. Touching edges do not count.
inline bool intersects(const Rect& a, const Rect& b) { return overlapArea(a, b) > 1e-9; }

// Euclidean distance between rectangle centers — used by the solver's scoring.
inline double centerDistance(const Rect& a, const Rect& b) {
  const Point ca = a.center();
  const Point cb = b.center();
  return std::hypot(ca.x - cb.x, ca.y - cb.y);
}

}  // namespace lattice
