#include "lattice/scene.hpp"

#include <sstream>

namespace lattice {

static std::string esc(const std::string& s) {
  std::string out;
  out.reserve(s.size());
  for (char c : s) {
    switch (c) {
      case '&': out += "&amp;"; break;
      case '<': out += "&lt;"; break;
      case '>': out += "&gt;"; break;
      case '"': out += "&quot;"; break;
      default: out += c;
    }
  }
  return out;
}

std::string emitSVG(const Scene& scene) {
  std::ostringstream o;
  o << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << scene.width << "\" height=\""
    << scene.height << "\" viewBox=\"0 0 " << scene.width << ' ' << scene.height << "\">\n";
  o << "  <rect x=\"0\" y=\"0\" width=\"" << scene.width << "\" height=\"" << scene.height
    << "\" fill=\"" << esc(scene.background) << "\"/>\n";
  for (const auto& n : scene.nodes) {
    switch (n.kind) {
      case NodeKind::Rect:
        o << "  <rect x=\"" << n.box.x << "\" y=\"" << n.box.y << "\" width=\"" << n.box.w
          << "\" height=\"" << n.box.h << "\" fill=\"" << esc(n.fill) << "\" stroke=\""
          << esc(n.stroke) << "\" stroke-width=\"" << n.strokeWidth << "\"/>\n";
        break;
      case NodeKind::Line:
      case NodeKind::Leader:
        o << "  <line x1=\"" << n.a.x << "\" y1=\"" << n.a.y << "\" x2=\"" << n.b.x << "\" y2=\""
          << n.b.y << "\" stroke=\"" << esc(n.stroke) << "\" stroke-width=\"" << n.strokeWidth
          << "\"/>\n";
        break;
      case NodeKind::Text: {
        const Point c = n.box.center();
        o << "  <text x=\"" << c.x << "\" y=\"" << (c.y + n.fontSize * 0.35) << "\" font-size=\""
          << n.fontSize << "\" fill=\"" << esc(n.textColor) << "\" text-anchor=\""
          << (n.textAnchorMiddle ? "middle" : "start") << "\" font-family=\"Arial, sans-serif\">"
          << esc(n.text) << "</text>\n";
        break;
      }
    }
  }
  o << "</svg>\n";
  return o.str();
}

}  // namespace lattice
