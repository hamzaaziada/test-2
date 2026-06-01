// Lattice core — platform-neutral scene graph (TRD §4.6) + SVG emitter.
// This is the "native-shape" stand-in: adapters translate these primitives 1:1 to
// real PowerPoint shapes (C1). Here we emit SVG so output is viewable/checkable headlessly.
#pragma once
#include <string>
#include <vector>

#include "lattice/geometry.hpp"

namespace lattice {

enum class NodeKind { Rect, Line, Text, Leader };

// One drawable primitive. A real adapter maps each to a native shape; the SVG emitter
// maps each to an SVG element. Z-order = vector order.
struct SceneNode {
  NodeKind kind = NodeKind::Rect;
  Rect box{};                  // for Rect / Text bounding box
  Point a{}, b{};              // for Line / Leader endpoints
  std::string fill = "none";   // hex like "#4472c4"
  std::string stroke = "none";
  double strokeWidth = 1.0;
  std::string text;            // for Text
  double fontSize = 12.0;
  std::string textColor = "#000000";
  bool textAnchorMiddle = true;
};

struct Scene {
  double width = 960.0;
  double height = 540.0;
  std::string background = "#ffffff";
  std::vector<SceneNode> nodes;

  void add(SceneNode n) { nodes.push_back(std::move(n)); }
};

// Serialize the scene to an SVG document string. Pure + deterministic.
std::string emitSVG(const Scene& scene);

}  // namespace lattice
