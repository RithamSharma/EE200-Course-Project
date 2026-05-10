# Task 009: Implement Windows Win32 screen capture backend

**depends-on**: task-003

## Description

Implement screen capture using Windows GDI+ or DirectX for Windows 10/11 systems.

## Execution Context

**Task Number**: 9 of 16
**Phase**: Platform Capture
**Prerequisites**: Task 003 (ScreenCapture base) must be complete

## BDD Scenario

```gherkin
Scenario: Windows Screen Capture
  Given Windows 10/11 (Win32/Win64)
  When `see` command is executed
  Then use GDI+ or DirectX for screen capture
  And convert to PNG format
  And handle multi-monitor setups
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Platform-Specific Screen Capture section

## Files to Modify/Create

- Create: `src/vision/platform/capture_win32.cpp`
- Create: `include/vision/platform/capture_win32.h`

## Steps

### Step 1: Define Win32Capture class
- Inherit from ScreenCapture
- Add Windows-specific members: HDC for screen DC

### Step 2: Implement capture() method
- Get device context for entire screen (GetDC(NULL))
- Create compatible DC and bitmap
- BitBlt from screen to compatible DC
- Get bitmap data
- Convert to ImageData

### Step 3: Implement multi-monitor support
- Enumerate monitors with EnumDisplayMonitors
- Allow selection of specific monitor

### Step 4: Implement is_supported() check
- Check if _WIN32 is defined
- Return true on Windows

## Interface Signatures

```cpp
class Win32Capture : public ScreenCapture {
public:
  Win32Capture();
  ~Win32Capture() override;
  
  Result<ImageData> capture() override;
  Result<ImageData> capture_monitor(int index);
  bool is_supported() const override;
  
private:
  Result<ImageData> capture_via_gdi();
};
```

## Verification Commands

```bash
# Compile on Windows (via cross-compilation or Windows environment)
g++ -std=c++20 -c src/vision/platform/capture_win32.cpp -I include -lgdi32

# Test on Windows
novashell.exe see
```

## Success Criteria

- Win32Capture inherits from ScreenCapture
- capture() returns ImageData from primary monitor
- capture_monitor(n) returns ImageData from specific monitor
- is_supported() returns true on Windows