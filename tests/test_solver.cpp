// Lattice solver — unit tests (Phase 0).
// Compile: g++ -std=c++20 -Icore/include core/src/solver.cpp tests/test_solver.cpp
//           -o /tmp/test_solver -Wall -Wextra && /tmp/test_solver
#include "lattice/solver.hpp"
#include "lattice/geometry.hpp"
#include "check.hpp"

#include <vector>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Returns true if none of the placed boxes overlap each other.
static bool noMutualOverlap(const std::vector<lattice::Placement>& placements) {
    for (std::size_t i = 0; i < placements.size(); ++i) {
        for (std::size_t j = i + 1; j < placements.size(); ++j) {
            if (lattice::intersects(placements[i].box, placements[j].box)) return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Test 1 — Non-colliding case
//   Two labels whose preferred candidates do NOT overlap.
//   Expected: both placed interior, no leader, no mutual overlap.
// ---------------------------------------------------------------------------
static void test_nonColliding() {
    // Label A: preferred slot on the left side of a 1000×500 canvas.
    lattice::LabelRequest a;
    a.id   = 1;
    a.anchor     = lattice::Rect{100, 200, 50, 20};
    a.candidates = {lattice::Rect{60, 195, 50, 20}};   // left of anchor, well-separated
    a.allowExternal = true;

    // Label B: preferred slot on the right side — far from A's slot.
    lattice::LabelRequest b;
    b.id   = 2;
    b.anchor     = lattice::Rect{800, 200, 50, 20};
    b.candidates = {lattice::Rect{860, 195, 50, 20}};  // right of anchor
    b.allowExternal = true;

    lattice::Rect canvas{0, 0, 1000, 500};
    auto results = lattice::placeLabels({a, b}, {}, canvas);

    // No-drop
    CHECK_EQ(static_cast<int>(results.size()), 2);

    // Both placed interior
    CHECK(!results[0].external);
    CHECK(!results[1].external);

    // No leaders
    CHECK(!results[0].leader.has_value());
    CHECK(!results[1].leader.has_value());

    // Correct ids preserved in order
    CHECK_EQ(results[0].id, 1);
    CHECK_EQ(results[1].id, 2);

    // Boxes are the preferred candidates
    CHECK_NEAR(results[0].box.x, 60.0,  1e-9);
    CHECK_NEAR(results[1].box.x, 860.0, 1e-9);

    // No mutual overlap
    CHECK(noMutualOverlap(results));
    CHECK_NEAR(lattice::overlapArea(results[0].box, results[1].box), 0.0, 1e-9);
}

// ---------------------------------------------------------------------------
// Test 2 — Forced-external case
//   Two labels whose only interior candidates are at exactly the same position
//   (total collision).  Each has a well-separated external candidate.
//   Expected: the second label goes external, both boxes do NOT overlap, and
//   the external placement carries a leader line.
// ---------------------------------------------------------------------------
static void test_forcedExternal() {
    // Both labels want to sit at the same rect {200,200,80,20}.
    lattice::Rect sharedSlot{200, 200, 80, 20};

    lattice::LabelRequest a;
    a.id = 10;
    a.anchor     = lattice::Rect{195, 195, 90, 30};
    a.candidates = {sharedSlot};
    a.allowExternal      = true;
    a.externalCandidates = {lattice::Rect{50, 400, 80, 20}};  // bottom-left, clear

    lattice::LabelRequest b;
    b.id = 11;
    b.anchor     = lattice::Rect{195, 195, 90, 30};  // same anchor as a
    b.candidates = {sharedSlot};                       // same interior slot — will collide
    b.allowExternal      = true;
    b.externalCandidates = {lattice::Rect{800, 400, 80, 20}};  // bottom-right, clear

    lattice::Rect canvas{0, 0, 1000, 500};
    auto results = lattice::placeLabels({a, b}, {}, canvas);

    CHECK_EQ(static_cast<int>(results.size()), 2);

    // First label wins the interior slot.
    CHECK(!results[0].external);
    CHECK(!results[0].leader.has_value());

    // Second label must be bumped to external.
    CHECK(results[1].external);
    CHECK(results[1].leader.has_value());

    // Leader endpoints are set correctly (anchor-center -> box-center).
    if (results[1].leader.has_value()) {
        lattice::Point expectedFrom = b.anchor.center();
        lattice::Point expectedTo   = results[1].box.center();
        CHECK_NEAR(results[1].leader->first.x,  expectedFrom.x, 1e-9);
        CHECK_NEAR(results[1].leader->first.y,  expectedFrom.y, 1e-9);
        CHECK_NEAR(results[1].leader->second.x, expectedTo.x,   1e-9);
        CHECK_NEAR(results[1].leader->second.y, expectedTo.y,   1e-9);
    }

    // The two final boxes must not overlap.
    CHECK_NEAR(lattice::overlapArea(results[0].box, results[1].box), 0.0, 1e-9);
    CHECK(noMutualOverlap(results));
}

// ---------------------------------------------------------------------------
// Test 3 — Obstacle avoidance
//   A label's first candidate collides with an obstacle; the solver must skip
//   to the second (free) candidate.
// ---------------------------------------------------------------------------
static void test_obstacleAvoidance() {
    // Obstacle occupies {300,100,100,50}.
    lattice::Rect obstacle{300, 100, 100, 50};

    lattice::LabelRequest req;
    req.id = 20;
    req.anchor = lattice::Rect{310, 110, 80, 30};
    // First candidate overlaps the obstacle; second is clear.
    req.candidates = {
        lattice::Rect{305, 105, 80, 30},   // overlaps obstacle
        lattice::Rect{500, 200, 80, 30}    // clear
    };
    req.allowExternal = false;

    lattice::Rect canvas{0, 0, 1000, 500};
    auto results = lattice::placeLabels({req}, {obstacle}, canvas);

    CHECK_EQ(static_cast<int>(results.size()), 1);
    CHECK(!results[0].external);
    CHECK(!results[0].leader.has_value());

    // Must have chosen the second candidate (clear one at x=500).
    CHECK_NEAR(results[0].box.x, 500.0, 1e-9);
    CHECK_NEAR(results[0].box.y, 200.0, 1e-9);

    // Placed box does not overlap the obstacle.
    CHECK_NEAR(lattice::overlapArea(results[0].box, obstacle), 0.0, 1e-9);
}

// ---------------------------------------------------------------------------
// Test 4 — Determinism
//   Calling placeLabels twice with the same input must yield identical results.
// ---------------------------------------------------------------------------
static void test_determinism() {
    lattice::LabelRequest a;
    a.id = 30;
    a.anchor = lattice::Rect{100, 100, 60, 20};
    a.candidates = {lattice::Rect{170, 95, 60, 20}, lattice::Rect{10, 95, 60, 20}};
    a.allowExternal      = true;
    a.externalCandidates = {lattice::Rect{50, 400, 60, 20}};

    lattice::LabelRequest b;
    b.id = 31;
    b.anchor = lattice::Rect{250, 100, 60, 20};
    b.candidates = {lattice::Rect{320, 95, 60, 20}, lattice::Rect{180, 95, 60, 20}};
    b.allowExternal      = true;
    b.externalCandidates = {lattice::Rect{700, 400, 60, 20}};

    lattice::Rect obs{160, 85, 70, 40};  // blocks some candidates
    lattice::Rect canvas{0, 0, 1000, 500};

    std::vector<lattice::LabelRequest> input{a, b};
    std::vector<lattice::Rect> obstacles{obs};

    auto r1 = lattice::placeLabels(input, obstacles, canvas);
    auto r2 = lattice::placeLabels(input, obstacles, canvas);

    CHECK_EQ(static_cast<int>(r1.size()), static_cast<int>(r2.size()));

    for (std::size_t i = 0; i < r1.size(); ++i) {
        CHECK_EQ(r1[i].id,       r2[i].id);
        CHECK_EQ(r1[i].external, r2[i].external);
        CHECK_NEAR(r1[i].box.x, r2[i].box.x, 1e-9);
        CHECK_NEAR(r1[i].box.y, r2[i].box.y, 1e-9);
        CHECK_NEAR(r1[i].box.w, r2[i].box.w, 1e-9);
        CHECK_NEAR(r1[i].box.h, r2[i].box.h, 1e-9);
        CHECK_EQ(r1[i].leader.has_value(), r2[i].leader.has_value());
    }
}

// ---------------------------------------------------------------------------
// Test 5 — No-drop guarantee
//   Output size always equals input size, even for a large batch.
// ---------------------------------------------------------------------------
static void test_noDrop() {
    // Pack 10 labels all wanting the same slot (worst case — many fallbacks).
    lattice::Rect canvas{0, 0, 1000, 500};
    std::vector<lattice::LabelRequest> requests;
    for (int i = 0; i < 10; ++i) {
        lattice::LabelRequest req;
        req.id = 100 + i;
        req.anchor = lattice::Rect{400, 240, 50, 20};
        req.candidates = {lattice::Rect{460, 235, 50, 20}};  // all want the same slot
        req.allowExternal = true;
        // Each label gets a unique external slot so at most one falls back completely.
        req.externalCandidates = {
            lattice::Rect{static_cast<double>(i * 60), 450, 50, 20}
        };
        requests.push_back(req);
    }

    auto results = lattice::placeLabels(requests, {}, canvas);
    CHECK_EQ(static_cast<int>(results.size()), 10);

    // Ids in output must match input order.
    for (int i = 0; i < 10; ++i) {
        CHECK_EQ(results[static_cast<std::size_t>(i)].id, 100 + i);
    }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main() {
    test_nonColliding();
    test_forcedExternal();
    test_obstacleAvoidance();
    test_determinism();
    test_noDrop();
    return lattice_test::summary();
}
