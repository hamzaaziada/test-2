// Lattice core — Layout constraint solver implementation (FR-B01/B02/C12).
// Algorithm: greedy sequential placement.
//   For each label (in input order), we scan its interior `candidates` first, then
//   (if allowExternal) its `externalCandidates`.  A candidate is ACCEPTABLE iff:
//     (1) canvas.contains(candidate)  — stays in-bounds
//     (2) !intersects(candidate, obstacle) for all obstacles  — no data-ink clash
//     (3) !intersects(candidate, placed_box) for all already-placed boxes — no label clash
//   The first acceptable candidate wins.  If nothing is found after both lists, we fall
//   back to candidates[0] (best-effort, no crash) so output.size() == input.size() always.
//   Complexity: O(L * (C + E) * (O + L)) where L=labels, C=interior candidates,
//               E=external candidates, O=obstacles.  Phase 0 target is fully acceptable.
//   Determinism: only std::vector iteration (stable order), no hash containers used.

#include "lattice/solver.hpp"
#include "lattice/geometry.hpp"

#include <vector>
#include <optional>
#include <utility>

namespace lattice {

std::vector<Placement> placeLabels(const std::vector<LabelRequest>& labels,
                                   const std::vector<Rect>& obstacles,
                                   const Rect& canvas) {
    std::vector<Placement> results;
    results.reserve(labels.size());

    // Accumulates the bounding boxes of all labels placed so far.
    std::vector<Rect> placedBoxes;
    placedBoxes.reserve(labels.size());

    // Helper: returns true if `candidate` is collision-free and in-canvas.
    auto isAcceptable = [&](const Rect& candidate) -> bool {
        // (1) Must be fully inside the canvas.
        if (!canvas.contains(candidate)) return false;

        // (2) Must not overlap any static obstacle.
        for (const Rect& obs : obstacles) {
            if (intersects(candidate, obs)) return false;
        }

        // (3) Must not overlap any already-placed label box.
        for (const Rect& placed : placedBoxes) {
            if (intersects(candidate, placed)) return false;
        }

        return true;
    };

    for (const LabelRequest& req : labels) {
        Placement p;
        p.id = req.id;

        bool found = false;

        // --- Phase 1: try interior candidates (preferred order) ---
        for (const Rect& c : req.candidates) {
            if (isAcceptable(c)) {
                p.box      = c;
                p.external = false;
                p.leader   = std::nullopt;
                found = true;
                break;
            }
        }

        // --- Phase 2: try external candidates if allowed and interior failed ---
        if (!found && req.allowExternal) {
            for (const Rect& c : req.externalCandidates) {
                if (isAcceptable(c)) {
                    p.box      = c;
                    p.external = true;
                    // Leader: from anchor center to label box center.
                    p.leader   = std::make_pair(req.anchor.center(), c.center());
                    found = true;
                    break;
                }
            }
        }

        // --- Phase 3: best-effort fallback — never drop a label ---
        if (!found) {
            // Fixtures are designed to be solvable; this path should not trigger
            // in passing tests, but we must never crash or shrink the output.
            p.box      = req.candidates[0];
            p.external = false;
            p.leader   = std::nullopt;
        }

        results.push_back(p);
        placedBoxes.push_back(p.box);
    }

    return results;
}

}  // namespace lattice
