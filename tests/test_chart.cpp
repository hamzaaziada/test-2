// Lattice — Chart geometry unit tests (FR-A04 + FR-A03).
#include "lattice/chart.hpp"
#include "lattice/datasheet.hpp"
#include "lattice/geometry.hpp"
#include "check.hpp"

#include <cmath>
#include <cstddef>
#include <string>

// -------------------------------------------------------------------------
// Helpers
// -------------------------------------------------------------------------

// Returns true when inner is contained within outer allowing eps tolerance.
static bool rectContainedWithEps(const lattice::Rect& outer,
                                 const lattice::Rect& inner,
                                 double eps) {
    return (inner.left()   >= outer.left()   - eps) &&
           (inner.right()  <= outer.right()  + eps) &&
           (inner.top()    >= outer.top()    - eps) &&
           (inner.bottom() <= outer.bottom() + eps);
}

int main() {
    using namespace lattice;

    // -----------------------------------------------------------------------
    // Test 1: buildStackedColumn with 3-series × 4-category data
    // -----------------------------------------------------------------------
    // Same dataset as datasheet tests for consistency.
    // Series 0: [10, 20, 30, 40]  color #ff0000
    // Series 1: [ 5, 15, 25, 35]  color #00ff00
    // Series 2: [ 2,  4,  6,  8]  color #0000ff
    // maxColumnTotal = 83 (column 3)
    // grandTotal = 200

    Datasheet ds;
    ds.categories = {"Q1", "Q2", "Q3", "Q4"};

    Series s0; s0.name = "A"; s0.color = "#ff0000"; s0.values = {10.0, 20.0, 30.0, 40.0};
    Series s1; s1.name = "B"; s1.color = "#00ff00"; s1.values = {5.0,  15.0, 25.0, 35.0};
    Series s2; s2.name = "C"; s2.color = "#0000ff"; s2.values = {2.0,  4.0,  6.0,  8.0};
    ds.series = {s0, s1, s2};

    const Rect area{0.0, 0.0, 800.0, 600.0};
    ChartLayout layout = buildStackedColumn(ds, area);

    // layout.plot must be strictly inside area
    CHECK(layout.plot.left()   >= area.left());
    CHECK(layout.plot.right()  <= area.right());
    CHECK(layout.plot.top()    >= area.top());
    CHECK(layout.plot.bottom() <= area.bottom());
    CHECK(layout.plot.w > 0.0);
    CHECK(layout.plot.h > 0.0);

    // All non-zero values: 3 series × 4 categories = 12 segments expected.
    CHECK_EQ(layout.segments.size(), static_cast<std::size_t>(12));

    // Every segment rect lies within the plot area (with small epsilon for fp).
    const double eps = 1e-6;
    for (const auto& seg : layout.segments) {
        CHECK(rectContainedWithEps(layout.plot, seg.rect, eps));
    }

    // The tallest column is column 3 (total = 83 = maxColumnTotal).
    // Its segments' heights must sum to approximately plot.h.
    // Segments are emitted in series order for each column; column 3 segments
    // are those with value 40, 35, 8 (in series order).
    // We find them by checking which segments have values matching column 3.
    // A simpler approach: sum segment heights for those at the x-position of column 3.
    // Column 3 slot: slotWidth = plot.w / 4, barLeft = plot.x + 3*slotWidth + barOffset.
    {
        const double slotWidth = layout.plot.w / 4.0;
        const double barWidth  = slotWidth * 0.60;
        const double barOffset = (slotWidth - barWidth) / 2.0;
        const double col3Left  = layout.plot.x + 3.0 * slotWidth + barOffset;

        double heightSum = 0.0;
        for (const auto& seg : layout.segments) {
            if (std::fabs(seg.rect.x - col3Left) < eps) {
                heightSum += seg.rect.h;
            }
        }
        CHECK_NEAR(heightSum, layout.plot.h, 1e-6);
    }

    // totalText has one entry per category.
    CHECK_EQ(layout.totalText.size(), static_cast<std::size_t>(4));
    CHECK_EQ(layout.totalAnchor.size(), static_cast<std::size_t>(4));

    // Column totals: 17, 39, 61, 83 (integers → no decimal).
    CHECK(layout.totalText[0] == std::string("17"));
    CHECK(layout.totalText[1] == std::string("39"));
    CHECK(layout.totalText[2] == std::string("61"));
    CHECK(layout.totalText[3] == std::string("83"));

    // totalAnchor rects must be above the plot area (or at least not below plot.top).
    for (const auto& anchor : layout.totalAnchor) {
        CHECK(anchor.bottom() <= layout.plot.top() + eps);
    }

    // -----------------------------------------------------------------------
    // Test 2: buildStackedColumn with a value that formats with 1 decimal.
    // -----------------------------------------------------------------------
    {
        Datasheet ds2;
        ds2.categories = {"X"};
        Series sx; sx.name = "X"; sx.color = "#aabbcc"; sx.values = {12.5};
        ds2.series = {sx};
        ChartLayout ly2 = buildStackedColumn(ds2, area);
        CHECK_EQ(ly2.segments.size(), static_cast<std::size_t>(1));
        CHECK(ly2.segments[0].label == std::string("12.5"));
        CHECK(ly2.totalText[0] == std::string("12.5"));
    }

    // -----------------------------------------------------------------------
    // Test 3: buildStackedColumn skips zero-value segments.
    // -----------------------------------------------------------------------
    {
        Datasheet ds3;
        ds3.categories = {"A", "B"};
        // Series with a zero value in column 1
        Series sa; sa.name = "S"; sa.color = "#123456"; sa.values = {10.0, 0.0};
        ds3.series = {sa};
        ChartLayout ly3 = buildStackedColumn(ds3, area);
        // Only 1 non-zero segment (the 10.0 in column 0; the 0.0 is skipped).
        CHECK_EQ(ly3.segments.size(), static_cast<std::size_t>(1));
    }

    // -----------------------------------------------------------------------
    // Test 4: buildWaterfall — basic functionality.
    // -----------------------------------------------------------------------
    // Deltas: [100, 50, -30, 20]
    // Running totals after each: 100, 150, 120, 140
    // All deltas non-zero → expect 4 segments.
    {
        Datasheet dw;
        dw.categories = {"Start", "+50", "-30", "+20"};
        Series sw; sw.name = "W"; sw.color = "#999999"; sw.values = {100.0, 50.0, -30.0, 20.0};
        dw.series = {sw};

        const Rect warea{0.0, 0.0, 600.0, 400.0};
        ChartLayout wl = buildWaterfall(dw, warea);

        CHECK_EQ(wl.segments.size(), static_cast<std::size_t>(4));

        // All segment rects must lie within the plot area.
        for (const auto& seg : wl.segments) {
            CHECK(rectContainedWithEps(wl.plot, seg.rect, 1e-6));
        }

        // Each segment must have positive height.
        for (const auto& seg : wl.segments) {
            CHECK(seg.rect.h > 0.0);
        }

        // Segment values match deltas.
        CHECK_NEAR(wl.segments[0].value, 100.0, 1e-9);
        CHECK_NEAR(wl.segments[1].value,  50.0, 1e-9);
        CHECK_NEAR(wl.segments[2].value, -30.0, 1e-9);
        CHECK_NEAR(wl.segments[3].value,  20.0, 1e-9);
    }

    // -----------------------------------------------------------------------
    // Test 5: buildWaterfall — does not crash on empty input.
    // -----------------------------------------------------------------------
    {
        Datasheet dempty;
        ChartLayout wle = buildWaterfall(dempty, area);
        CHECK_EQ(wle.segments.size(), static_cast<std::size_t>(0));
    }

    // -----------------------------------------------------------------------
    // Test 6: buildWaterfall — all-negative deltas.
    // -----------------------------------------------------------------------
    {
        Datasheet dneg;
        dneg.categories = {"A", "B"};
        Series sneg; sneg.name = "N"; sneg.color = "#ff0000"; sneg.values = {-10.0, -20.0};
        dneg.series = {sneg};
        const Rect na{0.0, 0.0, 400.0, 300.0};
        ChartLayout wln = buildWaterfall(dneg, na);
        CHECK_EQ(wln.segments.size(), static_cast<std::size_t>(2));
        for (const auto& seg : wln.segments) {
            CHECK(seg.rect.h > 0.0);
            CHECK(rectContainedWithEps(wln.plot, seg.rect, 1e-6));
        }
    }

    return lattice_test::summary();
}
