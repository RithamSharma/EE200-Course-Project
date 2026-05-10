# Task 003: Create ScreenCapture abstract base class

**depends-on**: task-002

## Description

Define the abstract base class for screen capture implementations with virtual `capture()` method and platform detection factory.

## Execution Context

**Task Number**: 3 of 16
**Phase**: Foundation
**Prerequisites**: Task 002 (types) must be complete

## BDD Scenario

```gherkin
Scenario: Platform detection and factory creation
  Given NovaShell is running on a specific platform
  When ScreenCapture::create_platform_capture() is called
  Then the correct platform-specific capture is returned
  And is_supported() returns true for the current platform
```

**Spec Source**: `../2026-05-11-mllm-vision-module/architecture.md` - Platform Detection Priority section

## Files to Modify/Create

- Create: `include/vision/screen_capture.h`
- Create: `src/vision/screen_capture.cpp`

## Steps

### Step 1: Define ScreenCapture abstract class
- Create abstract class with virtual `capture()` method returning `Result<ImageData>`
- Add virtual `is_supported() const` method
- Add virtual destructor

### Step 2: Define create_platform_capture factory
- Check environment variables in priority order:
  1. HYPRLAND_INSTANCE_SIGNATURE → HyprlandCapture
  2. WAYLAND_DISPLAY → WaylandCapture
  3. DISPLAY → X11Capture
  4. _WIN32 → Win32Capture
  5. __APPLE__ → MacCapture
- Return nullptr if no platform supported

### Step 3: Define platform-specific subclasses (forward declarations)
- X11Capture, WaylandCapture, HyprlandCapture, Win32Capture, MacCapture

## Interface Signatures

```cpp
class ScreenCapture {
public:
  virtual ~ScreenCapture() = default;
  virtual Result<ImageData> capture() = 0;
  virtual bool is_supported() const = 0;
  static std::unique_ptr<ScreenCapture> create_platform_capture();
};
```

## Verification Commands

```bash
# Compile screen capture base
g++ -std=c++20 -c src/vision/screen_capture.cpp -I include
```

## Success Criteria

- Abstract base class compiles
- Factory returns correct platform-specific capture
- All platform forward declarations exist