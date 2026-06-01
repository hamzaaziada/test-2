// Lattice core — Demo / reference artifact generator.
// Builds the reference stacked-column chart, places all labels, assembles a
// Scene, runs the oracle, writes lattice_reference.svg, and returns 0 on pass.
//
// Syntax-only check:
//   g++ -std=c++20 -Icore/include -Iverification/include -fsyntax-only core/src/demo_main.cpp
//
// Full build (from repo root) — combine all translation units and link.
#include "lattice/chart.hpp"
#include "lattice/datasheet.hpp"
#include "lattice/geometry.hpp"
#include "lattice/oracle.hpp"
#include "lattice/scene.hpp"
#include "lattice/solver.hpp"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Local copy of deriveLabelRequests (mirrors test_e2e_gate.cpp logic).
// ---------------------------------------------------------------------------
static std::vector<lattice::LabelRequest>
deriveLabelRequests(const lattice::ChartLayout& cl) {
    std::vector<lattice::LabelRequest> reqs;

    constexpr double LABEL_H    = 14.0;
    constexpr double CH_WIDTH   = 8.0;
    constexpr double MAX_LABEL_W = 80.0;

    const double extX = cl.plot.right() + 10.0;
    constexpr double EXT_STEP = 18.0;
    const std::size_t nSlots = cl.segments.size() + cl.totalText.size() + 4;

    std::vector<lattice::Rect> allExternalSlots;
    allExternalSlots.reserve(nSlots);
    for (std::size_t k = 0; k < nSlots; ++k) {
        allExternalSlots.push_back(lattice::Rect{
            extX,
            20.0 + static_cast<double>(k) * EXT_STEP,
            60.0,
            LABEL_H
        });
    }

    int nextId = 0;

    // Segment labels
    for (const auto& seg : cl.segments) {
        const double labelW =
            std::min(MAX_LABEL_W,
                     static_cast<double>(seg.label.size()) * CH_WIDTH + 4.0);
        const lattice::Point c = seg.rect.center();
        const lattice::Rect interior{
            c.x - labelW / 2.0,
            c.y - LABEL_H / 2.0,
            labelW,
            LABEL_H
        };
        lattice::LabelRequest req;
        req.id                = nextId++;
        req.anchor            = seg.rect;
        req.candidates        = {interior};
        req.allowExternal     = true;
        req.externalCandidates = allExternalSlots;
        reqs.push_back(std::move(req));
    }

    // Total labels
    for (std::size_t ti = 0; ti < cl.totalText.size(); ++ti) {
        const auto& txt    = cl.totalText[ti];
        const auto& anchor = cl.totalAnchor[ti];
        const double labelW =
            std::min(MAX_LABEL_W,
                     static_cast<double>(txt.size()) * CH_WIDTH + 4.0);
        const lattice::Point c = anchor.center();
        const lattice::Rect interior{
            c.x - labelW / 2.0,
            c.y - LABEL_H / 2.0,
            labelW,
            LABEL_H
        };
        lattice::LabelRequest req;
        req.id                = nextId++;
        req.anchor            = anchor;
        req.candidates        = {interior};
        req.allowExternal     = true;
        req.externalCandidates = allExternalSlots;
        reqs.push_back(std::move(req));
    }

    return reqs;
}

// ---------------------------------------------------------------------------
// Reference datasheet: 3 series × 4 categories (normal reference case).
// ---------------------------------------------------------------------------
static lattice::Datasheet makeReferenceDatasheet() {
    lattice::Datasheet ds;
    ds.categories = {"Q1", "Q2", "Q3", "Q4"};
    ds.series = {
        {"Alpha", {30.0, 40.0, 35.0, 45.0}, "#4472c4"},
        {"Beta",  {20.0, 25.0, 22.0, 28.0}, "#ed7d31"},
        {"Gamma", {15.0, 18.0, 20.0, 22.0}, "#a9d18e"},
    };
    return ds;
}

// ---------------------------------------------------------------------------
int main() {
    const lattice::Rect canvas{0.0, 0.0, 960.0, 540.0};

    // 1. Build chart geometry.
    const lattice::Datasheet ds = makeReferenceDatasheet();
    const lattice::ChartLayout cl = lattice::buildStackedColumn(ds, canvas);

    // 2. Derive label requests and run the solver.
    const auto reqs       = deriveLabelRequests(cl);
    const auto placements = lattice::placeLabels(reqs, {}, canvas);

    // 3. Run oracle on placed label boxes.
    std::vector<lattice::Rect> boxes;
    boxes.reserve(placements.size());
    for (const auto& p : placements) {
        boxes.push_back(p.box);
    }
    const lattice::OracleReport report = lattice::scoreLabels(boxes, canvas);
    std::fprintf(stderr, "%s\n", report.summary().c_str());

    // 4. Assemble the Scene.
    lattice::Scene scene;
    scene.width  = canvas.w;
    scene.height = canvas.h;
    scene.background = "#ffffff";

    // 4a. Plot background rectangle.
    {
        lattice::SceneNode bg;
        bg.kind        = lattice::NodeKind::Rect;
        bg.box         = cl.plot;
        bg.fill        = "#f5f5f5";
        bg.stroke      = "#cccccc";
        bg.strokeWidth = 0.5;
        scene.add(bg);
    }

    // 4b. One filled Rect per segment.
    for (const auto& seg : cl.segments) {
        lattice::SceneNode node;
        node.kind        = lattice::NodeKind::Rect;
        node.box         = seg.rect;
        node.fill        = seg.fill;
        node.stroke      = "#ffffff";
        node.strokeWidth = 0.5;
        scene.add(node);
    }

    // 4c. Label Text nodes + leader lines.
    // We need to map placement id back to the label text.
    // Ids 0..segments.size()-1 are segment labels; rest are total labels.
    const std::size_t numSegs = cl.segments.size();

    for (const auto& p : placements) {
        // Retrieve the label text for this placement id.
        std::string labelText;
        if (static_cast<std::size_t>(p.id) < numSegs) {
            labelText = cl.segments[static_cast<std::size_t>(p.id)].label;
        } else {
            const std::size_t totalIdx =
                static_cast<std::size_t>(p.id) - numSegs;
            if (totalIdx < cl.totalText.size()) {
                labelText = cl.totalText[totalIdx];
            }
        }

        // Leader line (only for external placements).
        if (p.external && p.leader.has_value()) {
            lattice::SceneNode leader;
            leader.kind        = lattice::NodeKind::Leader;
            leader.a           = p.leader->first;
            leader.b           = p.leader->second;
            leader.stroke      = "#888888";
            leader.strokeWidth = 0.75;
            scene.add(leader);
        }

        // Text node.
        lattice::SceneNode txt;
        txt.kind             = lattice::NodeKind::Text;
        txt.box              = p.box;
        txt.text             = labelText;
        txt.fontSize         = 11.0;
        txt.textColor        = "#222222";
        txt.textAnchorMiddle = true;
        scene.add(txt);
    }

    // 4d. Total Text nodes (anchored above each column).
    for (std::size_t ti = 0; ti < cl.totalText.size(); ++ti) {
        lattice::SceneNode tNode;
        tNode.kind             = lattice::NodeKind::Text;
        tNode.box              = cl.totalAnchor[ti];
        tNode.text             = cl.totalText[ti];
        tNode.fontSize         = 11.0;
        tNode.textColor        = "#000000";
        tNode.textAnchorMiddle = true;
        scene.add(tNode);
    }

    // 5. Write SVG to file.
    const std::string svgContent = lattice::emitSVG(scene);
    const char* svgPath = "lattice_reference.svg";
    std::ofstream ofs(svgPath);
    if (!ofs) {
        std::fprintf(stderr, "ERROR: cannot open %s for writing\n", svgPath);
        return 1;
    }
    ofs << svgContent;
    ofs.close();
    std::fprintf(stderr, "SVG written to %s\n", svgPath);

    // 6. Return 0 on oracle pass, 1 on fail.
    return report.pass ? 0 : 1;
}
