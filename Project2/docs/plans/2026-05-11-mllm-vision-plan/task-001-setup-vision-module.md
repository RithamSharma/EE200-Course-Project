# Task 001: Setup vision module project structure

**depends-on**: none

## Description

Create the project directory structure for the vision module, including header files for public interfaces and source files for implementations. Set up the include guards and basic namespace structure.

## Execution Context

**Task Number**: 1 of 16
**Phase**: Setup
**Prerequisites**: None

## BDD Scenario

This is a setup task that enables all subsequent development. No specific BDD scenario, but creates the foundation for all vision module features.

## Files to Create

```
include/vision/
├── vision.h              # Main vision module interface
├── screen_capture.h      # ScreenCapture abstract base
├── image_utils.h         # ImageProcessor utilities
└── analysis.h            # AnalysisMode enum and prompts

src/vision/
├── vision.cpp            # SeeCommand implementation
├── screen_capture.cpp    # Platform factory
├── image_utils.cpp       # ImageProcessor implementation
├── tui.cpp               # TUI interface
├── analysis.cpp          # Prompt crafting
└── platform/             # Platform-specific captures
    ├── capture_x11.cpp
    ├── capture_wayland.cpp
    ├── capture_hyprland.cpp
    ├── capture_win32.cpp
    └── capture_mac.mm

tests/
└── vision/
    ├── image_processor_test.cpp
    ├── error_handling_test.cpp
    └── integration_test.cpp
```

## Steps

### Step 1: Create directory structure
- Create all directories listed above
- Verify directories exist

### Step 2: Create stub headers
- Create `vision.h` with namespace `novashell::vision`
- Define basic forward declarations
- Add include guards (`#pragma once`)

### Step 3: Create stub source files
- Create empty `.cpp` files with basic includes
- Add placeholder implementations

### Step 4: Verify build
- Run `make` or compile test to ensure structure is valid

## Verification Commands

```bash
# Verify directory structure
ls -la include/vision/
ls -la src/vision/

# Verify compilation
make 2>&1 | head -20
```

## Success Criteria

- All directories created
- Headers compile without errors
- Basic project structure is valid