# Task 008: Implement Hyprland screen capture backend

**depends-on**: task-003

## Description

Implement screen capture using Hyprland's internal screenshot API for Linux systems running Hyprland compositor.

## Execution Context

**Task Number**: 8 of 16
**Phase**: Platform Capture
**Prerequisites**: Task 003 (ScreenCapture base) must be complete

## BDD Scenario

```gherkin
Scenario: Hyprland Screen Capture
  Given Linux with Hyprland compositor
  When `see` command is executed
  Then use Hyprland's internal screenshot API
  And convert to PNG format
  And use grim as fallback
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Platform-Specific Screen Capture section

## Files to Modify/Create

- Create: `src/vision/platform/capture_hyprland.cpp`
- Create: `include/vision/platform/capture_hyprland.h`

## Steps

### Step 1: Define HyprlandCapture class
- Inherit from ScreenCapture
- Check for HYPRLAND_INSTANCE_SIGNATURE env var

### Step 2: Implement Hyprland native capture
- Use Hyprland's IPC mechanism for screenshots
- Parse JSON response with image data

### Step 3: Implement grim fallback
- Execute `grim` command as fallback
- Parse output file path
- Load image from file

### Step 4: Implement is_supported() check
- Check HYPRLAND_INSTANCE_SIGNATURE environment variable

## Interface Signatures

```cpp
class HyprlandCapture : public ScreenCapture {
public:
  HyprlandCapture();
  ~HyprlandCapture() override;
  
  Result<ImageData> capture() override;
  bool is_supported() const override;
  
private:
  Result<ImageData> capture_via_hyprctl();
  Result<ImageData> capture_via_grim();
  std::string get_hyprsocket_path();
};
```

## Verification Commands

```bash
# Compile Hyprland capture
g++ -std=c++20 -c src/vision/platform/capture_hyprland.cpp -I include

# Test on Hyprland system
./novashell see
```

## Success Criteria

- HyprlandCapture inherits from ScreenCapture
- Uses Hyprland's hyprctl for native capture when available
- Falls back to grim on failure
- is_supported() returns true when HYPRLAND_INSTANCE_SIGNATURE is set