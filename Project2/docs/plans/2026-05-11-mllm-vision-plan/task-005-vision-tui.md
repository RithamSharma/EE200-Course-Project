# Task 005: Create TUI interface for image preview and region selection

**depends-on**: task-004

## Description

Implement the interactive TUI for displaying captured images, allowing region selection with arrow keys, and presenting analysis mode menu.

## Execution Context

**Task Number**: 5 of 16
**Phase**: Core Features
**Prerequisites**: Task 004 (ImageProcessor) should be complete for image display utilities

## BDD Scenario

```gherkin
Scenario: Region Selection with TUI
  Given NovaShell is running with valid Gemini API authentication
  When the user types `see` and enters capture mode
  Then the system displays captured screen in TUI
  And allows arrow keys to select a region via overlay
  And shows coordinates and dimensions in real-time
  And pressing Enter confirms selection
  And only the selected region is sent for analysis
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Vision Screen Analysis section

## Files to Modify/Create

- Create: `include/vision/tui.h`
- Create: `src/vision/tui.cpp`

## Steps

### Step 1: Define TUI interface class
- Create class with methods for:
  - `display_image(const ImageData&)` - show captured image
  - `select_region()` - return selected region coordinates
  - `show_mode_menu()` - present analysis mode options
  - `show_progress(const std::string&)` - display loading indicators

### Step 2: Implement ASCII preview
- Convert image to ASCII representation for terminal display
- Support resize for large images

### Step 3: Implement region selection
- Draw selection overlay on image
- Handle arrow key input for moving/resizing
- Display coordinates and dimensions

### Step 4: Implement mode selection menu
- Show numbered options for analysis modes
- Handle keyboard input for selection

## Interface Signatures

```cpp
struct Region {
  int x, y, width, height;
};

class VisionTUI {
public:
  VisionTUI();
  ~VisionTUI();
  
  void display_image(const ImageData& img);
  Region select_region();
  AnalysisMode select_mode();
  void show_progress(const std::string& message);
  void show_error(const std::string& message);
};
```

## Verification Commands

```bash
# Compile TUI
g++ -std=c++20 -c src/vision/tui.cpp -I include

# Manual test (requires terminal)
./novashell see
```

## Success Criteria

- TUI displays captured image
- Arrow keys move selection overlay
- Enter confirms selection with coordinates
- Mode menu shows all 4 analysis options