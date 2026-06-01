// Lattice — Annotation engine unit tests (FR-C01 CAGR).
#include "lattice/annotation.hpp"
#include "check.hpp"

#include <cmath>
#include <string>

int main() {
    using namespace lattice;

    // -----------------------------------------------------------------------
    // Test 1: CAGR of 100 -> 200 over 3 periods.
    // Expected: cagr = 2^(1/3) - 1 ≈ 0.25992...
    // Formatted: "26.0% p.a."
    // -----------------------------------------------------------------------
    {
        CagrResult r = computeCagr(100.0, 200.0, 3);
        CHECK(r.valid);
        CHECK_NEAR(r.cagr, 0.25992, 1e-3);
        CHECK(r.text == std::string("26.0% p.a."));
    }

    // -----------------------------------------------------------------------
    // Test 2: CAGR of 100 -> 121 over 2 periods.
    // Expected: cagr = (121/100)^(1/2) - 1 = 1.1 - 1 = 0.1 (10.0%)
    // Formatted: "10.0% p.a."
    // -----------------------------------------------------------------------
    {
        CagrResult r = computeCagr(100.0, 121.0, 2);
        CHECK(r.valid);
        CHECK_NEAR(r.cagr, 0.1, 1e-9);
        CHECK(r.text == std::string("10.0% p.a."));
    }

    // -----------------------------------------------------------------------
    // Test 3: Custom suffix.
    // -----------------------------------------------------------------------
    {
        CagrResult r = computeCagr(100.0, 200.0, 3, "CAGR");
        CHECK(r.valid);
        CHECK(r.text == std::string("26.0% CAGR"));
    }

    // -----------------------------------------------------------------------
    // Test 4: Invalid inputs → valid = false.
    // -----------------------------------------------------------------------
    // first <= 0
    {
        CagrResult r = computeCagr(0.0, 200.0, 3);
        CHECK(!r.valid);
    }
    {
        CagrResult r = computeCagr(-10.0, 200.0, 3);
        CHECK(!r.valid);
    }
    // last <= 0
    {
        CagrResult r = computeCagr(100.0, 0.0, 3);
        CHECK(!r.valid);
    }
    {
        CagrResult r = computeCagr(100.0, -50.0, 3);
        CHECK(!r.valid);
    }
    // periods <= 0
    {
        CagrResult r = computeCagr(100.0, 200.0, 0);
        CHECK(!r.valid);
    }
    {
        CagrResult r = computeCagr(100.0, 200.0, -1);
        CHECK(!r.valid);
    }

    // -----------------------------------------------------------------------
    // Test 5: Single period (periods = 1) — cagr = last/first - 1.
    // -----------------------------------------------------------------------
    {
        CagrResult r = computeCagr(50.0, 75.0, 1);
        CHECK(r.valid);
        CHECK_NEAR(r.cagr, 0.5, 1e-9);
        CHECK(r.text == std::string("50.0% p.a."));
    }

    // -----------------------------------------------------------------------
    // Test 6: Shrinkage — last < first.
    // CAGR of 200 -> 100 over 1 period = -50%.
    // -----------------------------------------------------------------------
    {
        CagrResult r = computeCagr(200.0, 100.0, 1);
        CHECK(r.valid);
        CHECK_NEAR(r.cagr, -0.5, 1e-9);
        CHECK(r.text == std::string("-50.0% p.a."));
    }

    return lattice_test::summary();
}
