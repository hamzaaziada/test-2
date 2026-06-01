# ADR 0002 — Core language (C++20) and the Phase-0 native-shape stand-in (SVG)

- **Status:** Accepted (Phase 0)
- **Context tasks:** T-0004 (core-language spike, TQ-5), T-0003 (native-shape emission spike), T-0020/T-0021 (scene-graph + codec)
- **Deciders:** Principal Architect, OOXML/Serialization Engineer

## Problem

(a) Pick the core language. (b) The real degradation target (C1) is **native PowerPoint shapes** emitted via the Windows/Mac adapters, which require real Office and cannot be built or verified in a headless Linux CI container. Phase 0 still needs a *viewable, checkable* output to prove the geometry+solver pipeline end-to-end.

## Decision

1. **Core language = C++20 + CMake** (TRD §4 mandate; g++ 13 available). Rust+C-ABI remains the named alternative but is not adopted — COM-ecosystem fit and a single source tree win for parity with the incumbent's structure.
2. **Phase-0 output = the platform-neutral scene graph (`scene.hpp`) serialized to SVG.** SVG is a faithful 1:1 stand-in for the adapter's native-shape emission: every `SceneNode` (Rect/Line/Text/Leader) maps to exactly one native shape in a real adapter, and to one SVG element here. This lets the layout-quality oracle (FR-I01) read real geometry and lets a human eyeball the reference chart **without** needing Office.

## Consequences

- (+) The entire Phase-0 moat (data → geometry → solver → scene → oracle) builds and runs its checks headlessly in CI.
- (+) The scene graph is the seam: swapping the SVG emitter for a COM/OOXML emitter (the Windows/Mac adapters) is an adapter change, not a core change.
- (−) **Native-object graceful degradation (C1/FR-H02/FR-I05) is NOT verified by Phase 0** — it requires the real adapters on real Office (R-1/R-2). This is an explicit, documented gap, carried as Gate-1/Gate-2 work, not silently claimed. (Master Directive 6: honesty over optimism.)
