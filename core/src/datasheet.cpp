// Lattice core — Datasheet implementation (FR-A02/A03).
// Deterministic, no I/O.
#include "lattice/datasheet.hpp"

#include <algorithm>
#include <cstddef>

namespace lattice {

std::size_t Datasheet::categoryCount() const {
    return categories.size();
}

double Datasheet::columnTotal(std::size_t categoryIndex) const {
    double total = 0.0;
    for (const auto& s : series) {
        if (categoryIndex < s.values.size()) {
            total += s.values[categoryIndex];
        }
        // missing values treated as 0 (out-of-range guard)
    }
    return total;
}

double Datasheet::maxColumnTotal() const {
    if (categories.empty()) return 0.0;
    double mx = 0.0;
    for (std::size_t i = 0; i < categories.size(); ++i) {
        mx = std::max(mx, columnTotal(i));
    }
    return mx;
}

double Datasheet::grandTotal() const {
    double total = 0.0;
    for (const auto& s : series) {
        for (const double v : s.values) {
            total += v;
        }
    }
    return total;
}

}  // namespace lattice
