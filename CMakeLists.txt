cmake_minimum_required(VERSION 3.20)
project(Lattice LANGUAGES CXX)

# Spec-faithful: TRD §4 mandates C++20 + CMake.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

# ---- Portable headless core (TRD §4) ----
# demo_main.cpp carries main() + needs verification headers; it is an executable, not lib code.
file(GLOB CORE_SOURCES CONFIGURE_DEPENDS core/src/*.cpp)
list(FILTER CORE_SOURCES EXCLUDE REGEX "core/src/demo_main\\.cpp$")
add_library(lattice_core STATIC ${CORE_SOURCES})
target_include_directories(lattice_core PUBLIC core/include)

# ---- Verification machinery (Epic I): oracle, equivalence helpers ----
file(GLOB VERIFY_SOURCES CONFIGURE_DEPENDS verification/src/*.cpp)
add_library(lattice_verify STATIC ${VERIFY_SOURCES})
target_include_directories(lattice_verify PUBLIC verification/include)
target_link_libraries(lattice_verify PUBLIC lattice_core)

# ---- Demo / artifact generator (renders the Phase-0 reference chart to SVG) ----
if(EXISTS ${CMAKE_SOURCE_DIR}/core/src/demo_main.cpp)
  add_executable(lattice_demo core/src/demo_main.cpp)
  target_link_libraries(lattice_demo PRIVATE lattice_core lattice_verify)
endif()

# ---- Tests (CTest). Every tests/test_*.cpp becomes a registered test. ----
enable_testing()
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/test_*.cpp)
foreach(test_src ${TEST_SOURCES})
  get_filename_component(test_name ${test_src} NAME_WE)
  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE lattice_core lattice_verify)
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
