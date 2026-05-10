# Task 007: Implement Wayland screen capture backend

**depends-on**: task-003

## Description

Implement screen capture using Wayland screencopy protocol for Linux systems running Wayland compositors.

## Execution Context

**Task Number**: 7 of 16
**Phase**: Platform Capture
**Prerequisites**: Task 003 (ScreenCapture base) must be complete

## BDD Scenario

```gherkin
Scenario: Wayland Screen Capture
  Given Linux with Wayland compositor
  When `see` command is executed
  Then use screencopy protocol via libwayland
  And convert to PNG format
  And fallback to gnome-screenshot if protocol unavailable
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Platform-Specific Screen Capture section

## Files to Modify/Create

- Create: `src/vision/platform/capture_wayland.cpp`
- Create: `include/vision/platform/capture_wayland.h`

## Steps

### Step 1: Define WaylandCapture class
- Inherit from ScreenCapture
- Add Wayland-specific members: wl_display*, wl_shm*

### Step 2: Implement screencopy protocol
- Connect to Wayland compositor
- Request screencopy buffer
- Wait for buffer ready callback

### Step 3: Implement fallback
- Check if screencopy fails
- Execute `gnome-screenshot` as fallback
- Parse output to ImageData

### Step 4: Implement is_supported() check
- Check if WAYLAND_DISPLAY is set
- Exclude Hyprland (handled separately)

## Interface Signatures

```cpp
class WaylandCapture : public ScreenCapture {
public:
  WaylandCapture();
  ~WaylandCapture() override;
  
  Result<ImageData> capture() override;
  bool is_supported() const override;
  
private:
  Result<ImageData> capture_via_screencopy();
  Result<ImageData> capture_via_gnome_screenshot();
};
```

## Verification Commands

```bash
# Compile Wayland capture
g++ -std=c++20 -c src/vision/platform/capture_wayland.cpp -I include -lwayland-client

# Test on Wayland system
./novashell see
```

## Success Criteria

- WaylandCapture inherits from ScreenCapture
- Uses screencopy protocol when available
- Falls back to gnome-screenshot on failure
- is_supported() returns true on Wayland (non-Hyprland)