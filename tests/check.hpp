// Tiny zero-dependency test harness (no GoogleTest needed in this environment).
// Usage: CHECK(cond); CHECK_EQ(a,b); at end call return lattice_test::summary();
#pragma once
#include <cmath>
#include <cstdio>
#include <string>

namespace lattice_test {
inline int& failures() {
  static int f = 0;
  return f;
}
inline int& total() {
  static int t = 0;
  return t;
}
inline void record(bool ok, const char* expr, const char* file, int line) {
  ++total();
  if (!ok) {
    ++failures();
    std::fprintf(stderr, "  FAIL: %s  (%s:%d)\n", expr, file, line);
  }
}
inline int summary() {
  std::fprintf(stderr, "  %d/%d checks passed\n", total() - failures(), total());
  return failures() == 0 ? 0 : 1;
}
}  // namespace lattice_test

#define CHECK(cond) ::lattice_test::record((cond), #cond, __FILE__, __LINE__)
#define CHECK_EQ(a, b) ::lattice_test::record((a) == (b), #a " == " #b, __FILE__, __LINE__)
#define CHECK_NEAR(a, b, eps) \
  ::lattice_test::record(std::fabs((a) - (b)) <= (eps), #a " ~= " #b, __FILE__, __LINE__)
