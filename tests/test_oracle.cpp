// Lattice QA — Unit tests for the layout-quality oracle (FR-I01 / FR-I03).
// Compile + run:
//   g++ -std=c++20 -Icore/include -Iverification/include
//       verification/src/oracle.cpp tests/test_oracle.cpp
//       -o /tmp/t_or -Wall -Wextra && /tmp/t_or
#include "lattice/oracle.hpp"
#include "lattice/geometry.hpp"
#include "check.hpp"

#include <cstdio>
#include <vector>

// ---------------------------------------------------------------------------
// Helper: build a canvas covering the whole slide reference area.
// ---------------------------------------------------------------------------
static lattice::Rect makeCanvas() {
    return lattice::Rect{0.0, 0.0, 960.0, 540.0};
}

// ---------------------------------------------------------------------------
// Test 1 — Two clearly overlapping boxes in-canvas.
//   Expected: pass==false, overlappingPairs>=1, totalOverlapArea>0.
// ---------------------------------------------------------------------------
static void test_overlapping_boxes() {
    std::fprintf(stderr, "test_overlapping_boxes\n");
    const lattice::Rect canvas = makeCanvas();

    // Box A: (100,100) 80×30
    // Box B: (140,110) 80×30  — overlaps A significantly
    std::vector<lattice::Rect> boxes = {
        {100.0, 100.0, 80.0, 30.0},
        {140.0, 110.0, 80.0, 30.0},
    };
    const auto r = lattice::scoreLabels(boxes, canvas);

    CHECK(r.pass == false);
    CHECK(r.overlappingPairs >= 1);
    CHECK(r.totalOverlapArea > 0.0);
    CHECK(r.offCanvasLabels == 0);  // both are in-canvas
}

// ---------------------------------------------------------------------------
// Test 2 — Two disjoint in-canvas boxes.
//   Expected: pass==true, overlappingPairs==0, offCanvasLabels==0.
// ---------------------------------------------------------------------------
static void test_disjoint_incanvas_boxes() {
    std::fprintf(stderr, "test_disjoint_incanvas_boxes\n");
    const lattice::Rect canvas = makeCanvas();

    // Box A: (50,50) 80×20 — right edge at 130
    // Box B: (200,50) 80×20 — clearly separated
    std::vector<lattice::Rect> boxes = {
        { 50.0, 50.0, 80.0, 20.0},
        {200.0, 50.0, 80.0, 20.0},
    };
    const auto r = lattice::scoreLabels(boxes, canvas);

    CHECK(r.pass == true);
    CHECK_EQ(r.overlappingPairs, 0);
    CHECK_EQ(r.offCanvasLabels, 0);
    CHECK(r.totalOverlapArea == 0.0);
}

// ---------------------------------------------------------------------------
// Test 3 — A box partly outside the canvas.
//   Expected: offCanvasLabels>=1, pass==false.
// ---------------------------------------------------------------------------
static void test_box_off_canvas() {
    std::fprintf(stderr, "test_box_off_canvas\n");
    const lattice::Rect canvas = makeCanvas();

    // Box hangs 20 px off the right edge of the 960-wide canvas.
    std::vector<lattice::Rect> boxes = {
        {900.0, 50.0, 80.0, 20.0},  // right edge = 980 > 960 → off-canvas
    };
    const auto r = lattice::scoreLabels(boxes, canvas);

    CHECK(r.offCanvasLabels >= 1);
    CHECK(r.pass == false);
}

// ---------------------------------------------------------------------------
// Test 4 — Empty box list.
//   Expected: everything zero, pass==true (vacuously).
// ---------------------------------------------------------------------------
static void test_empty() {
    std::fprintf(stderr, "test_empty\n");
    const lattice::Rect canvas = makeCanvas();
    const auto r = lattice::scoreLabels({}, canvas);

    CHECK(r.pass == true);
    CHECK_EQ(r.overlappingPairs, 0);
    CHECK_EQ(r.offCanvasLabels, 0);
    CHECK(r.alignmentScore == 1.0);
}

// ---------------------------------------------------------------------------
// Test 5 — Single box fully inside canvas.
//   Expected: pass==true, alignmentScore==1.0 (trivial single-box case).
// ---------------------------------------------------------------------------
static void test_single_incanvas() {
    std::fprintf(stderr, "test_single_incanvas\n");
    const lattice::Rect canvas = makeCanvas();
    std::vector<lattice::Rect> boxes = {{200.0, 200.0, 60.0, 14.0}};
    const auto r = lattice::scoreLabels(boxes, canvas);

    CHECK(r.pass == true);
    CHECK(r.alignmentScore == 1.0);
}

// ---------------------------------------------------------------------------
// Test 6 — summary() string sanity.
// ---------------------------------------------------------------------------
static void test_summary_string() {
    std::fprintf(stderr, "test_summary_string\n");
    const lattice::Rect canvas = makeCanvas();
    std::vector<lattice::Rect> boxes = {
        {50.0, 50.0, 60.0, 14.0},
        {50.0, 80.0, 60.0, 14.0},   // same left edge → aligned
    };
    const auto r = lattice::scoreLabels(boxes, canvas);
    const std::string s = r.summary();

    // Must contain oracle: prefix and PASS/FAIL keyword.
    CHECK(s.find("oracle:") != std::string::npos);
    CHECK(s.find("overlaps=") != std::string::npos);
    CHECK(s.find("offCanvas=") != std::string::npos);
    CHECK(s.find("align=") != std::string::npos);
    std::fprintf(stderr, "  summary: %s\n", s.c_str());
}

// ---------------------------------------------------------------------------
// Test 7 — Three-way overlap: A overlaps B, B overlaps C, A does not overlap C.
//   Expected: overlappingPairs==2.
// ---------------------------------------------------------------------------
static void test_three_way_partial_overlap() {
    std::fprintf(stderr, "test_three_way_partial_overlap\n");
    const lattice::Rect canvas = makeCanvas();

    // A: x=0..80,  B: x=50..130, C: x=120..200
    // A∩B > 0, B∩C > 0, A∩C == 0
    std::vector<lattice::Rect> boxes = {
        {  0.0, 50.0, 80.0, 20.0},
        { 50.0, 50.0, 80.0, 20.0},
        {120.0, 50.0, 80.0, 20.0},
    };
    const auto r = lattice::scoreLabels(boxes, canvas);

    CHECK_EQ(r.overlappingPairs, 2);
    CHECK(r.totalOverlapArea > 0.0);
    CHECK(r.pass == false);
}

// ---------------------------------------------------------------------------
int main() {
    test_overlapping_boxes();
    test_disjoint_incanvas_boxes();
    test_box_off_canvas();
    test_empty();
    test_single_incanvas();
    test_summary_string();
    test_three_way_partial_overlap();
    return lattice_test::summary();
}
