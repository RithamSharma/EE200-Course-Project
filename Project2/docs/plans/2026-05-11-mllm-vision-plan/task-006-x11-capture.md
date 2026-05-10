# Task 006: Implement X11 screen capture backend

**depends-on**: task-003

## Description

Implement screen capture using X11/Xlib APIs for Linux systems running X11.

## Execution Context

**Task Number**: 6 of 16
**Phase**: Platform Capture
**Prerequisites**: Task 003 (ScreenCapture base) must be complete

## BDD Scenario

```gherkin
Scenario: X11 Screen Capture
  Given Linux with X11 session
  When `see` command is executed
  Then use xwd/xlib to capture root window
  And convert to PNG format
  And return image data for processing
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Platform-Specific Screen Capture section

## Files to Modify/Create

- Create: `src/vision/platform/capture_x11.cpp`
- Create: `include/vision/platform/capture_x11.h`

## Steps

### Step 1: Define X11Capture class
- Inherit from ScreenCapture
- Add X11-specific members: Display*, Window root

### Step 2: Implement capture() method
- Open X11 display connection
- Get root window dimensions
- Use XGetImage to capture root window
- Convert XImage to ImageData
- Handle color format (RGB/BGR conversion)

### Step 3: Implement is_supported() check
- Check if DISPLAY environment variable is set
- Try to open X11 connection

### Step 4: Add error handling
- Handle X11 connection failures
- Handle permission denied errors
- Clean up resources properly

## Interface Signatures

```cpp
class X11Capture : public ScreenCapture {
public:
  X11Capture();
  ~X11Capture() override;
  
  Result<ImageData> capture() override;
  bool is_supported() const override;
  
private:
  Display* display_;
  Window root_;
};
```

## Verification Commands

```bash
# Compile X11 capture
g++ -std=c++20 -c src/vision/platform/capture_x11.cpp -I include -lX11

# Test on X11 system
./novashell see
```

## Success Criteria

- X11Capture inherits from ScreenCapture
- capture() returns ImageData with valid pixels
- is_supported() returns true on X11 systems
- Proper cleanup on destruction