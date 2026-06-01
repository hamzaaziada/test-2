// Lattice QA — End-to-end collision gate (FR-I03 keystone check).
// Wires the full Phase-0 pipeline:
//   Datasheet -> buildStackedColumn/buildWaterfall -> deriveLabelRequests
//   -> placeLabels -> scoreLabels -> CHECK(report.pass)
//
// Syntax-only verification (headers compile clean):
//   g++ -std=c++20 -Icore/include -Iverification/include -fsyntax-only tests/test_e2e_gate.cpp
//
// Full link + execution is performed by the orchestrator as the integration gate.
#include "lattice/chart.hpp"
#include "lattice/datasheet.hpp"
#include "lattice/geometry.hpp"
#include "lattice/oracle.hpp"
#include "lattice/solver.hpp"
#include "check.hpp"

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// deriveLabelRequests — converts a ChartLayout into LabelRequests ready for
// the solver.  Each segment gets one interior candidate (label centered in the
// segment) and a column of well-separated external candidates to the right of
// the plot area.  Total labels get the same treatment.
// ---------------------------------------------------------------------------
static std::vector<lattice::LabelRequest>
deriveLabelRequests(const lattice::ChartLayout& cl) {
    std::vector<lattice::LabelRequest> reqs;

    // Label dimensions: approximate text box.
    constexpr double LABEL_H   = 14.0;
    constexpr double CH_WIDTH  = 8.0;   // pixels per character (rough)
    constexpr double MAX_LABEL_W = 80.0;

    // External slot column: stacked vertically starting just to the right of
    // the plot area.  Slots are 18 px apart so they are NEVER mutually
    // overlapping.  We generate enough slots for all segments + totals.
    const double extX = cl.plot.right() + 10.0;
    constexpr double EXT_STEP = 18.0;   // slot height stride (> LABEL_H → no overlap)
    const std::size_t nSlots =
        cl.segments.size() + cl.totalText.size() + 4;  // generous headroom

    // Pre-build the full external slot list once; each LabelRequest references
    // the same vector (copies are cheap — they are just Rects).
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

    // --- Segment labels ---
    for (const auto& seg : cl.segments) {
        const double labelW =
            std::min(MAX_LABEL_W,
                     static_cast<double>(seg.label.size()) * CH_WIDTH + 4.0);

        // Interior candidate: centered in the segment rect.
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

    // --- Total labels ---
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
// Fixture helpers
// ---------------------------------------------------------------------------

// (a) Normal 3-series × 4-category stacked column chart.
static lattice::Datasheet makeNormal3x4() {
    lattice::Datasheet ds;
    ds.categories = {"Q1", "Q2", "Q3", "Q4"};
    ds.series = {
        {"Alpha", {30.0, 40.0, 35.0, 45.0}, "#4472c4"},
        {"Beta",  {20.0, 25.0, 22.0, 28.0}, "#ed7d31"},
        {"Gamma", {15.0, 18.0, 20.0, 22.0}, "#a9d18e"},
    };
    return ds;
}

// (b) Tiny-segment case: one series has a very small value that forces the
// interior label to collide with its neighbors → solver must use external slot.
static lattice::Datasheet makeTinySegment() {
    lattice::Datasheet ds;
    ds.categories = {"Jan", "Feb", "Mar"};
    ds.series = {
        {"Big",   {80.0, 85.0, 90.0}, "#4472c4"},
        {"Small", { 2.0,  2.0,  2.0}, "#ed7d31"},  // tiny segments
        {"Mid",   {40.0, 42.0, 44.0}, "#a9d18e"},
    };
    return ds;
}

// (c) Dense case: 8 categories × 4 series — many labels must be squeezed in.
static lattice::Datasheet makeDense8x4() {
    lattice::Datasheet ds;
    ds.categories = {"A","B","C","D","E","F","G","H"};
    ds.series = {
        {"S1", {10.0,12.0,11.0,13.0,14.0,10.0,12.0,11.0}, "#4472c4"},
        {"S2", { 8.0, 9.0, 7.0, 8.0, 9.0, 8.0, 7.0, 9.0}, "#ed7d31"},
        {"S3", { 6.0, 7.0, 6.0, 5.0, 8.0, 6.0, 5.0, 7.0}, "#a9d18e"},
        {"S4", { 4.0, 5.0, 4.0, 5.0, 4.0, 5.0, 4.0, 5.0}, "#ffc000"},
    };
    return ds;
}

// (d) Waterfall: single-series delta values for a simple running-total chart.
static lattice::Datasheet makeWaterfall() {
    lattice::Datasheet ds;
    ds.categories = {"Start","+Sales","-COGS","+Other","-Tax","End"};
    ds.series = {
        {"Delta", {100.0, 40.0, -30.0, 15.0, -20.0, 0.0}, "#4472c4"},
    };
    return ds;
}

// ---------------------------------------------------------------------------
// run one fixture through the full pipeline
// ---------------------------------------------------------------------------
struct FixtureResult {
    std::string name;
    lattice::OracleReport report;
};

static FixtureResult runStackedFixture(const std::string& name,
                                       const lattice::Datasheet& ds,
                                       const lattice::Rect& canvas) {
    const lattice::ChartLayout cl  = lattice::buildStackedColumn(ds, canvas);
    const auto reqs                = deriveLabelRequests(cl);
    const auto placements          = lattice::placeLabels(reqs, {}, canvas);

    std::vector<lattice::Rect> boxes;
    boxes.reserve(placements.size());
    for (const auto& p : placements) {
        boxes.push_back(p.box);
    }

    FixtureResult fr;
    fr.name   = name;
    fr.report = lattice::scoreLabels(boxes, canvas);
    return fr;
}

static FixtureResult runWaterfallFixture(const std::string& name,
                                         const lattice::Datasheet& ds,
                                         const lattice::Rect& canvas) {
    const lattice::ChartLayout cl  = lattice::buildWaterfall(ds, canvas);
    const auto reqs                = deriveLabelRequests(cl);
    const auto placements          = lattice::placeLabels(reqs, {}, canvas);

    std::vector<lattice::Rect> boxes;
    boxes.reserve(placements.size());
    for (const auto& p : placements) {
        boxes.push_back(p.box);
    }

    FixtureResult fr;
    fr.name   = name;
    fr.report = lattice::scoreLabels(boxes, canvas);
    return fr;
}

// ---------------------------------------------------------------------------
int main() {
    const lattice::Rect canvas{0.0, 0.0, 960.0, 540.0};

    std::vector<FixtureResult> results;

    // (a) Normal 3×4
    results.push_back(runStackedFixture("normal_3x4",     makeNormal3x4(),  canvas));
    // (b) Tiny-segment → forces external + leader
    results.push_back(runStackedFixture("tiny_segment",   makeTinySegment(), canvas));
    // (c) Dense 8×4
    results.push_back(runStackedFixture("dense_8x4",      makeDense8x4(),   canvas));
    // (d) Waterfall
    results.push_back(runWaterfallFixture("waterfall",    makeWaterfall(),  canvas));

    // --- Gate checks ---
    for (const auto& fr : results) {
        std::fprintf(stderr, "[%s] %s\n", fr.name.c_str(), fr.report.summary().c_str());

        CHECK(fr.report.pass);
        CHECK_EQ(fr.report.overlappingPairs, 0);
        CHECK_EQ(fr.report.offCanvasLabels, 0);
    }

    return lattice_test::summary();
}
