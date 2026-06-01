// Lattice verification — Layout-quality oracle implementation (FR-I01 / FR-I03).
// Implemented by QA / Verification Lead (T-B012).
// scoreLabels is the hard collision gate: pass iff overlappingPairs==0 && offCanvasLabels==0.
#include "lattice/oracle.hpp"
#include "lattice/geometry.hpp"

#include <cstdio>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

namespace lattice {

OracleReport scoreLabels(const std::vector<Rect>& labelBoxes, const Rect& canvas) {
    OracleReport report;
    const std::size_t n = labelBoxes.size();

    // --- overlapping pairs + total overlap area ---
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            const double oa = overlapArea(labelBoxes[i], labelBoxes[j]);
            if (oa > 1e-9) {
                ++report.overlappingPairs;
                report.totalOverlapArea += oa;
            }
        }
    }

    // --- off-canvas labels ---
    for (const Rect& box : labelBoxes) {
        if (!canvas.contains(box)) {
            ++report.offCanvasLabels;
        }
    }

    // --- alignment score ---
    // Fraction of boxes whose left edge aligns (within 0.5 px) with at least one OTHER box's
    // left edge. If 0 or 1 boxes, score is trivially 1.0.
    if (n <= 1) {
        report.alignmentScore = 1.0;
    } else {
        int aligned = 0;
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j < n; ++j) {
                if (i == j) continue;
                if (std::fabs(labelBoxes[i].left() - labelBoxes[j].left()) <= 0.5) {
                    ++aligned;
                    break;  // at least one match found for box i
                }
            }
        }
        report.alignmentScore = static_cast<double>(aligned) / static_cast<double>(n);
    }

    // --- pass gate (FR-I03 hard gate) ---
    report.pass = (report.overlappingPairs == 0 && report.offCanvasLabels == 0);

    return report;
}

std::string OracleReport::summary() const {
    std::ostringstream oss;
    oss << "oracle: " << (pass ? "PASS" : "FAIL")
        << " overlaps=" << overlappingPairs
        << " offCanvas=" << offCanvasLabels
        << " align=" << std::fixed << std::setprecision(2) << alignmentScore;
    return oss.str();
}

}  // namespace lattice
