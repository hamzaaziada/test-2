# .ci — Continuous checks (Master Part E)

Phase-0 CI is `cmake -S . -B build && cmake --build build && ctest`. The collision gate
(`test_e2e_gate`, FR-I03) and the layout-quality oracle (FR-I01) run here; overlap > 0 fails
the build. The cross-Office grid (FR-I04, T-2051) is added in Phase 2.
