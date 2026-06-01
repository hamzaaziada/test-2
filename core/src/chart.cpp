// Lattice core — Chart geometry implementation (FR-A04 + FR-A03 totals).
// Pure functions: (data, area) -> shapes.
#include "lattice/chart.hpp"

#include <cmath>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <string>

namespace lattice {

// -------------------------------------------------------------------------
// Internal helper: format a value for display.
// Integers (fractional part < 1e-9) print with no decimals; otherwise 1 dp.
// -------------------------------------------------------------------------
static std::string formatValue(double v) {
    double intpart = 0.0;
    double fracpart = std::modf(v, &intpart);
    std::ostringstream oss;
    if (std::fabs(fracpart) < 1e-9) {
        oss << std::fixed << std::setprecision(0) << v;
    } else {
        oss << std::fixed << std::setprecision(1) << v;
    }
    return oss.str();
}

// -------------------------------------------------------------------------
// buildStackedColumn
// -------------------------------------------------------------------------
ChartLayout buildStackedColumn(const Datasheet& data, const Rect& area) {
    ChartLayout layout;

    // --- Inset margins (as fractions of area dimensions) ---
    // Left/bottom ~12% for axes, top ~8% for total labels.
    const double marginLeft   = area.w * 0.12;
    const double marginBottom = area.h * 0.12;
    const double marginTop    = area.h * 0.08;
    const double marginRight  = area.w * 0.04;

    layout.plot = Rect{
        area.x + marginLeft,
        area.y + marginTop,
        area.w - marginLeft - marginRight,
        area.h - marginTop - marginBottom
    };

    const std::size_t numCats = data.categoryCount();
    if (numCats == 0) return layout;

    const double maxTotal = data.maxColumnTotal();
    // Avoid division by zero when all values are 0.
    const double scale = (maxTotal > 1e-15) ? layout.plot.h / maxTotal : 0.0;

    // Each category gets an equal slot; bars occupy 60% of the slot, centered.
    const double slotWidth = layout.plot.w / static_cast<double>(numCats);
    const double barWidth  = slotWidth * 0.60;
    const double barOffset = (slotWidth - barWidth) / 2.0;

    // Reserve space for totals.
    layout.totalText.resize(numCats);
    layout.totalAnchor.resize(numCats);

    for (std::size_t ci = 0; ci < numCats; ++ci) {
        const double slotLeft = layout.plot.x + static_cast<double>(ci) * slotWidth;
        const double barLeft  = slotLeft + barOffset;

        // Stack from the bottom of the plot area upward.
        double runningHeight = 0.0;  // accumulated height drawn so far

        for (const auto& s : data.series) {
            double value = 0.0;
            if (ci < s.values.size()) {
                value = s.values[ci];
            }

            // Skip zero-height segments.
            if (std::fabs(value) < 1e-15) continue;

            const double segH = value * scale;
            // In slide coordinates y grows downward, so the top of this
            // segment is (plot.bottom - runningHeight - segH).
            const double segTop = layout.plot.bottom() - runningHeight - segH;

            Segment seg;
            seg.rect  = Rect{barLeft, segTop, barWidth, segH};
            seg.fill  = s.color;
            seg.value = value;
            seg.label = formatValue(value);
            layout.segments.push_back(seg);

            runningHeight += segH;
        }

        // Total label: sits just above the column, inside the top margin.
        const double colTotal = data.columnTotal(ci);
        layout.totalText[ci]  = formatValue(colTotal);

        // Anchor rect: horizontally centered on bar, vertically in the gap
        // between plot.top and the top of the column.
        const double totalLabelH = marginTop * 0.6;
        const double totalLabelY = layout.plot.top() - totalLabelH - (marginTop * 0.1);
        layout.totalAnchor[ci] = Rect{
            barLeft,
            totalLabelY,
            barWidth,
            totalLabelH
        };
    }

    return layout;
}

// -------------------------------------------------------------------------
// buildWaterfall
// -------------------------------------------------------------------------
ChartLayout buildWaterfall(const Datasheet& data, const Rect& area) {
    ChartLayout layout;

    // Guard: need at least one series with at least one value.
    if (data.series.empty()) return layout;
    const auto& deltas = data.series[0].values;
    if (deltas.empty()) return layout;

    // --- Inset margins ---
    const double marginLeft   = area.w * 0.12;
    const double marginBottom = area.h * 0.12;
    const double marginTop    = area.h * 0.08;
    const double marginRight  = area.w * 0.04;

    layout.plot = Rect{
        area.x + marginLeft,
        area.y + marginTop,
        area.w - marginLeft - marginRight,
        area.h - marginTop - marginBottom
    };

    // Compute running totals to determine the y-axis range.
    const std::size_t n = deltas.size();
    double minVal = 0.0;
    double maxVal = 0.0;
    double running = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        const double prev = running;
        running += deltas[i];
        minVal = std::min({minVal, prev, running});
        maxVal = std::max({maxVal, prev, running});
    }

    const double range = maxVal - minVal;
    const double scale = (range > 1e-15) ? layout.plot.h / range : 0.0;

    const double slotWidth = layout.plot.w / static_cast<double>(n);
    const double barWidth  = slotWidth * 0.60;
    const double barOffset = (slotWidth - barWidth) / 2.0;

    const std::string& color = data.series[0].color;
    running = 0.0;

    for (std::size_t i = 0; i < n; ++i) {
        const double delta = deltas[i];
        const double from  = running;
        const double to    = running + delta;
        running = to;

        // Bar top/bottom in value space → convert to slide coordinates.
        // plot.bottom() corresponds to minVal; plot.top() to maxVal.
        const double barTop    = layout.plot.bottom() - (std::max(from, to) - minVal) * scale;
        const double barBottom = layout.plot.bottom() - (std::min(from, to) - minVal) * scale;
        const double barH      = barBottom - barTop;

        if (barH < 1e-15) continue;  // zero delta — skip

        const double barLeft = layout.plot.x + static_cast<double>(i) * slotWidth + barOffset;

        Segment seg;
        seg.rect  = Rect{barLeft, barTop, barWidth, barH};
        seg.fill  = color;
        seg.value = delta;
        seg.label = formatValue(delta);
        layout.segments.push_back(seg);
    }

    // totalText / totalAnchor left empty for Phase 0 as per spec.
    return layout;
}

}  // namespace lattice
