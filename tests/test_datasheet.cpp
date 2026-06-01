// Lattice — Datasheet unit tests (FR-A02/A03).
#include "lattice/datasheet.hpp"
#include "check.hpp"

#include <cstddef>

int main() {
    using namespace lattice;

    // Build a 3-series × 4-category datasheet with known values.
    // Series 0: [10, 20, 30, 40]
    // Series 1: [ 5, 15, 25, 35]
    // Series 2: [ 2,  4,  6,  8]
    //
    // columnTotal(0) = 10 +  5 +  2 = 17
    // columnTotal(1) = 20 + 15 +  4 = 39
    // columnTotal(2) = 30 + 25 +  6 = 61
    // columnTotal(3) = 40 + 35 +  8 = 83
    // maxColumnTotal = 83
    // grandTotal     = 17 + 39 + 61 + 83 = 200

    Datasheet ds;
    ds.categories = {"Q1", "Q2", "Q3", "Q4"};

    Series s0;
    s0.name   = "A";
    s0.color  = "#ff0000";
    s0.values = {10.0, 20.0, 30.0, 40.0};

    Series s1;
    s1.name   = "B";
    s1.color  = "#00ff00";
    s1.values = {5.0, 15.0, 25.0, 35.0};

    Series s2;
    s2.name   = "C";
    s2.color  = "#0000ff";
    s2.values = {2.0, 4.0, 6.0, 8.0};

    ds.series = {s0, s1, s2};

    // categoryCount
    CHECK_EQ(ds.categoryCount(), static_cast<std::size_t>(4));

    // columnTotal
    CHECK_NEAR(ds.columnTotal(0), 17.0, 1e-9);
    CHECK_NEAR(ds.columnTotal(1), 39.0, 1e-9);
    CHECK_NEAR(ds.columnTotal(2), 61.0, 1e-9);
    CHECK_NEAR(ds.columnTotal(3), 83.0, 1e-9);

    // out-of-range category index treated as 0 (missing values = 0)
    CHECK_NEAR(ds.columnTotal(99), 0.0, 1e-9);

    // maxColumnTotal
    CHECK_NEAR(ds.maxColumnTotal(), 83.0, 1e-9);

    // grandTotal
    CHECK_NEAR(ds.grandTotal(), 200.0, 1e-9);

    // Empty datasheet edge cases
    Datasheet empty;
    CHECK_EQ(empty.categoryCount(), static_cast<std::size_t>(0));
    CHECK_NEAR(empty.maxColumnTotal(), 0.0, 1e-9);
    CHECK_NEAR(empty.grandTotal(), 0.0, 1e-9);

    // Datasheet with categories but no series
    Datasheet noSeries;
    noSeries.categories = {"X", "Y"};
    CHECK_EQ(noSeries.categoryCount(), static_cast<std::size_t>(2));
    CHECK_NEAR(noSeries.columnTotal(0), 0.0, 1e-9);
    CHECK_NEAR(noSeries.maxColumnTotal(), 0.0, 1e-9);

    return lattice_test::summary();
}
