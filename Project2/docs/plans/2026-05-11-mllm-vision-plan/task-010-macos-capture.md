# Task 010: Implement macOS screen capture backend

**depends-on**: task-003

## Description

Implement screen capture using CoreGraphics/CGDisplayCapture for macOS systems.

## Execution Context

**Task Number**: 10 of 16
**Phase**: Platform Capture
**Prerequisites**: Task 003 (ScreenCapture base) must be complete

## BDD Scenario

```gherkin
Scenario: macOS Screen Capture
  Given macOS
  When `see` command is executed
  Then use CGDisplayCapture/CoreGraphics
  And convert to PNG format
  And handle Retina display scaling
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Platform-Specific Screen Capture section

## Files to Modify/Create

- Create: `src/vision/platform/capture_mac.cpp` (or .mm for Objective-C++)
- Create: `include/vision/platform/capture_mac.h`

## Steps

### Step 1: Define MacCapture class
- Inherit from ScreenCapture
- Use CGDirectDisplayID for display handling

### Step 2: Implement capture() method
- Get main display ID with CGMainDisplayID()
- Use CGDisplayCreateImage() to capture
- Handle Retina scaling (2x pixel density)

### Step 3: Handle Retina displays
- Detect Retina displays via display mode
- Return appropriate resolution (not scaled twice)

### Step 4: Implement is_supported() check
- Check if __APPLE__ is defined
- Return true on macOS

## Interface Signatures

```cpp
class MacCapture : public ScreenCapture {
public:
  MacCapture();
  ~MacCapture() override;
  
  Result<ImageData> capture() override;
  Result<ImageData> capture_display(CGDirectDisplayID display);
  bool is_supported() const override;
  
private:
  Result<ImageData> cgimage_to_imagedata(CGImageRef img);
};
```

## Verification Commands

```bash
# Compile on macOS
g++ -std=c++20 -c src/vision/platform/capture_mac.cpp -I include -framework CoreGraphics -framework ApplicationServices

# Test on macOS
./novashell see
```

## Success Criteria

- MacCapture inherits from ScreenCapture
- capture() returns ImageData from main display
- Handles Retina display scaling correctly
- is_supported() returns true on macOS