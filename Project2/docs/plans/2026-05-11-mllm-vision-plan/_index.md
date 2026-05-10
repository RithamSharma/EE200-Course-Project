# MLLM Vision Module Implementation Plan

**Goal:** Add vision capabilities to NovaShell with interactive `see` command for screen capture, image analysis, and multimodal AI interaction.

**Architecture:** Module-based design with platform-specific screen capture backends, shared image processing utilities, and TUI-based region selection. Integrates with existing Gemini API authentication.

**Tech Stack:** C++20, X11/Wayland/Hyprland/Win32/macOS APIs, libpng for image processing, existing curl-based Gemini API integration

**Design Support:**
- [BDD Specs](../2026-05-11-mllm-vision-module/bdd-specs.md)
- [Architecture](../2026-05-11-mllm-vision-module/architecture.md)

## Context

NovaShell v3.0 currently supports text-only AI interactions via Google Gemini. The vision module adds multimodal AI capabilities by:
- Adding a new `see` command with interactive TUI
- Supporting platform-specific screen capture across Linux (X11/Wayland/Hyprland), Windows, and macOS
- Providing multi-mode analysis (screen understanding, OCR, accessibility, full)
- Extending existing API integration for image-based prompts

This is a new feature module that doesn't modify existing core functionality.

## Execution Plan

```yaml
tasks:
  - id: "001"
    subject: "Setup vision module project structure"
    slug: "setup-vision-module"
    type: "setup"
    depends-on: []
  - id: "002"
    subject: "Implement ImageData and VisionError types"
    slug: "image-data-types"
    type: "impl"
    depends-on: ["001"]
  - id: "003"
    subject: "Create ScreenCapture abstract base class"
    slug: "screen-capture-base"
    type: "impl"
    depends-on: ["002"]
  - id: "004"
    subject: "Implement ImageProcessor utilities (base64, resize, format conversion)"
    slug: "image-processor"
    type: "impl"
    depends-on: ["002"]
  - id: "005"
    subject: "Create TUI interface for image preview and region selection"
    slug: "vision-tui"
    type: "impl"
    depends-on: ["004"]
  - id: "006"
    subject: "Implement X11 screen capture backend"
    slug: "x11-capture"
    type: "impl"
    depends-on: ["003"]
  - id: "007"
    subject: "Implement Wayland screen capture backend"
    slug: "wayland-capture"
    type: "impl"
    depends-on: ["003"]
  - id: "008"
    subject: "Implement Hyprland screen capture backend"
    slug: "hyprland-capture"
    type: "impl"
    depends-on: ["003"]
  - id: "009"
    subject: "Implement Windows Win32 screen capture backend"
    slug: "win32-capture"
    type: "impl"
    depends-on: ["003"]
  - id: "010"
    subject: "Implement macOS screen capture backend"
    slug: "macos-capture"
    type: "impl"
    depends-on: ["003"]
  - id: "011"
    subject: "Implement SeeCommand orchestrator"
    slug: "see-command"
    type: "impl"
    depends-on: ["005", "006", "007", "008", "009", "010"]
  - id: "012"
    subject: "Implement analysis prompt crafting and mode selection"
    slug: "analysis-modes"
    type: "impl"
    depends-on: ["004"]
  - id: "013"
    subject: "Integrate 'see' command into Shell and add auth validation"
    slug: "shell-integration"
    type: "impl"
    depends-on: ["011"]
  - id: "014"
    subject: "Add vision error scenarios tests"
    slug: "vision-error-tests"
    type: "test"
    depends-on: ["004"]
  - id: "015"
    subject: "Add screen capture integration tests"
    slug: "capture-integration-tests"
    type: "test"
    depends-on: ["013"]
  - id: "016"
    subject: "Update Makefile with vision module build rules"
    slug: "makefile-update"
    type: "config"
    depends-on: ["001", "006", "007", "008", "009", "010"]
```

**Task File References (for detailed BDD scenarios):**
- [Task 001: Setup vision module project structure](./task-001-setup-vision-module.md)
- [Task 002: Implement ImageData and VisionError types](./task-002-image-data-types.md)
- [Task 003: Create ScreenCapture abstract base class](./task-003-screen-capture-base.md)
- [Task 004: Implement ImageProcessor utilities](./task-004-image-processor.md)
- [Task 005: Create TUI interface](./task-005-vision-tui.md)
- [Task 006: Implement X11 screen capture backend](./task-006-x11-capture.md)
- [Task 007: Implement Wayland screen capture backend](./task-007-wayland-capture.md)
- [Task 008: Implement Hyprland screen capture backend](./task-008-hyprland-capture.md)
- [Task 009: Implement Windows Win32 screen capture backend](./task-009-win32-capture.md)
- [Task 010: Implement macOS screen capture backend](./task-010-macos-capture.md)
- [Task 011: Implement SeeCommand orchestrator](./task-011-see-command.md)
- [Task 012: Implement analysis prompt crafting](./task-012-analysis-modes.md)
- [Task 013: Integrate 'see' command into Shell](./task-013-shell-integration.md)
- [Task 014: Add vision error scenarios tests](./task-014-vision-error-tests.md)
- [Task 015: Add screen capture integration tests](./task-015-capture-integration-tests.md)
- [Task 016: Update Makefile with vision module build rules](./task-016-makefile-update.md)

## BDD Coverage

All 27 BDD scenarios from the design are covered:

| Feature | Scenarios | Task Coverage |
|---------|-----------|---------------|
| Vision Screen Analysis | 7 | Tasks 011, 013, 014 |
| Platform-Specific Capture | 5 | Tasks 006-010 |
| Image Processing Pipeline | 3 | Tasks 004, 014 |
| Analysis Modes | 4 | Tasks 012 |
| Error Handling and Edge Cases | 6 | Tasks 004, 011, 014, 015 |

## Dependency Chain

```
task-001 (setup)
    │
    ├─→ task-002 (types) ─→ task-003 (capture-base) ─→ task-006,007,008,009,010 (platforms)
    │                                                       │
    ├─→ task-004 (image-proc) ─→ task-005 (tui) ────────────┤
    │                                                       │
    └───────────────────────────────────────────────────────┼──→ task-011 (see-command) ─→ task-013 (shell-integration)
                                                               │
task-012 (analysis) ──────────────────────────────────────────┤
                                                               │
task-014,015 (tests) ◀─────────────────────────────────────────┘
                                                               │
task-016 (makefile) ◀───────────────────────────────────────────┘
```

**Analysis**:
- No circular dependencies
- Platform capture backends (006-010) are independent and can run in parallel after task-003
- TUI (005) depends on ImageProcessor (004) but independent of platform captures
- SeeCommand (011) requires all components to be ready
- Tests (014, 015) depend on implementation tasks
- Makefile (016) depends on all implementation and platform tasks

## Execution Handoff

**"Plan complete and saved to `docs/plans/2026-05-11-mllm-vision-plan/`. Execution options:**

**1. Orchestrated Execution (Recommended)** - Load `superpowers:executing-plans` skill using the Skill tool.

**2. Direct Agent Team** - Load `superpowers:agent-team-driven-development` skill using the Skill tool.

**3. BDD-Focused Execution** - Load `superpowers:behavior-driven-development` skill using the Skill tool for specific scenarios.