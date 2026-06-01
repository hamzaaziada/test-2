// Lattice core — Annotation engine implementation (FR-C01 CAGR).
#include "lattice/annotation.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace lattice {

CagrResult computeCagr(double first, double last, int periods, const std::string& suffix) {
    CagrResult result;

    if (first <= 0.0 || last <= 0.0 || periods <= 0) {
        result.valid = false;
        return result;
    }

    result.cagr  = std::pow(last / first, 1.0 / static_cast<double>(periods)) - 1.0;
    result.valid = true;

    // Format: one decimal place percent + space + suffix, e.g. "12.3% p.a."
    const double pct = result.cagr * 100.0;
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << pct << "% " << suffix;
    result.text = oss.str();

    return result;
}

}  // namespace lattice
