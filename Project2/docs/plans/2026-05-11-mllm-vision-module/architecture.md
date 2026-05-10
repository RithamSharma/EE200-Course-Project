# MLLM Vision Module - Architecture

## Component Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        Shell (existing)                    │
│                          calls 'see'                        │
└──────────────────────────────┬──────────────────────────────┘
                               │
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                     SeeCommand (vision.cpp)                 │
│  - Orchestrates capture → process → analyze flow          │
│  - Manages TUI interaction                                  │
│  - Handles user input and mode selection                    │
└──────────────────────┬──────────────────────┬────────────────┘
                       │                      │
          ┌────────────┴───────┐    ┌────────┴────────┐
          ▼                    ▼    ▼                 ▼
┌─────────────────┐  ┌─────────────────┐  ┌────────────────────┐
│  ImageProcessor │  │  AnalysisEngine │  │   TUI Interface    │
│  - Base64 encode│  │  - Mode selection│  │  - Image preview  │
│  - Compress     │  │  - Prompt craft │  │  - Region select   │
│  - Format conv  │  │  - Result format│  │  - Mode menu       │
└─────────────────┘  └─────────────────┘  └────────────────────┘
                               │
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                   ScreenCapture (abstract)                   │
│                    virtual capture() -> ImageData           │
└────────────────┬─────────────┬──────────────┬───────────────┘
     ┌───────────┴────┐       │       ┌───────┴──────┐
     │                │       │       │              │
     ▼                ▼       ▼       ▼              ▼
┌─────────┐   ┌───────────┐ ┌──────────┐  ┌──────────┐ ┌──────────┐
│  X11    │   │  Wayland  │ │ Hyprland │  │  Win32   │ │  macOS   │
│Capture  │   │  Capture  │ │ Capture  │  │  Capture │ │  Capture │
└─────────┘   └───────────┘ └──────────┘  └──────────┘ └──────────┘
```

## Class Definitions

### SeeCommand

```cpp
class SeeCommand {
public:
  int run(std::optional<std::string> image_path);
  
private:
  ImageData capture_screen();
  ImageData load_image_file(const std::string& path);
  ImageData select_region(ImageData& img);
  std::string analyze(ImageData& img, AnalysisMode mode);
  void display_result(const std::string& result);
};
```

### ScreenCapture (Abstract Base)

```cpp
class ScreenCapture {
public:
  virtual ~ScreenCapture() = default;
  virtual ImageData capture() = 0;
  virtual bool is_supported() const = 0;
  static std::unique_ptr<ScreenCapture> create_platform_capture();
};
```

### ImageProcessor

```cpp
class ImageProcessor {
public:
  static std::string to_base64(const ImageData& img);
  static ImageData resize(const ImageData& img, int max_w, int max_h);
  static ImageData convert_to_png(const ImageData& img);
};
```

### AnalysisMode

```cpp
enum class AnalysisMode {
  ScreenUnderstanding,
  OcrTextExtraction,
  AccessibilityAudit,
  FullAnalysis
};
```

## File Structure

```
include/vision/
├── vision.h              # SeeCommand, ImageData, AnalysisMode
├── screen_capture.h      # ScreenCapture base class
├── image_utils.h         # ImageProcessor utilities
└── platform/
    ├── capture_x11.h
    ├── capture_wayland.h
    ├── capture_hyprland.h
    ├── capture_win32.h
    └── capture_mac.h

src/vision/
├── vision.cpp            # SeeCommand implementation
├── screen_capture.cpp    # Factory and platform detection
├── image_utils.cpp      # ImageProcessor implementation
├── tui.cpp              # TUI for preview and selection
├── analysis.cpp         # Prompt crafting and result parsing
└── platform/
    ├── capture_x11.cpp
    ├── capture_wayland.cpp
    ├── capture_hyprland.cpp
    ├── capture_win32.cpp
    └── capture_mac.mm
```

## Build Configuration

```cmake
# Add to existing CMakeLists.txt
set(VISION_SOURCES
    src/vision/vision.cpp
    src/vision/screen_capture.cpp
    src/vision/image_utils.cpp
    src/vision/tui.cpp
    src/vision/analysis.cpp
)

# Platform-specific
if(LINUX_X11)
    list(APPEND VISION_SOURCES src/vision/platform/capture_x11.cpp)
    target_link_libraries(novashell X11 Xext)
endif()

if(LINUX_WAYLAND)
    list(APPEND VISION_SOURCES src/vision/platform/capture_wayland.cpp)
    target_link_libraries(novashell wayland-client)
endif()

if(LINUX_HYPRLAND)
    list(APPEND VISION_SOURCES src/vision/platform/capture_hyprland.cpp)
endif()
```

## API Integration

Extends `Shell::call_gemini_api()` for vision:

```cpp
std::string call_gemini_vision(const ImageData& img, const std::string& prompt) {
  std::string base64 = ImageProcessor::to_base64(img);
  
  // Modified JSON with inlineData
  std::string json = fmt::format(R"({{
    "contents": [{{
      "parts": [
        {{"text": "{}"}},
        {{"inlineData": {{
          "mimeType": "image/png",
          "data": "{}"
        }}}}
      ]
    }}]
  }})", prompt, base64);
  
  return call_gemini_api_json(json);
}
```

## TUI Design

```
┌─────────────────────────────────────────────────────────────┐
│                    NovaShell Vision                         │
├─────────────────────────────────────────────────────────────┤
│  [Captured Image Preview - ASCII/Framebuffer]               │
│                                                             │
│                                                             │
├─────────────────────────────────────────────────────────────┤
│  Analysis Mode:                                             │
│  [1] Screen Understanding    [2] OCR Text                  │
│  [3] Accessibility Audit      [4] Full Analysis              │
├─────────────────────────────────────────────────────────────┤
│  Region: x=120 y=80 w=400 h=300    [Arrow keys] Move        │
│  [Enter] Confirm  [r] Recapture  [q] Quit                   │
└─────────────────────────────────────────────────────────────┘
```

## Memory Management

- ImageData contains raw pixel buffer (RGBA)
- Move semantics for efficient transfer between stages
- Release buffer immediately after API call
- Max memory: ~8MB for full HD capture (1920x1080x4)

## Threading Model

- Capture runs on main thread (TUI interaction)
- API call runs on main thread with loading indicator
- Future: async API call with cancellable operation

## Platform Detection Priority

Detection order is critical for Linux systems running multiple display servers:

```cpp
std::unique_ptr<ScreenCapture> ScreenCapture::create_platform_capture() {
  // 1. Hyprland (runs on Wayland but needs special handling)
  if (getenv("HYPRLAND_INSTANCE_SIGNATURE"))
    return std::make_unique<HyprlandCapture>();
  
  // 2. Other Wayland compositors (GNOME, KDE, etc.)
  if (getenv("WAYLAND_DISPLAY"))
    return std::make_unique<WaylandCapture>();
  
  // 3. X11 (fallback for legacy systems)
  if (getenv("DISPLAY"))
    return std::make_unique<X11Capture>();
  
  // 4. Windows (Win32 API)
  #ifdef _WIN32
    return std::make_unique<Win32Capture>();
  #endif
  
  // 5. macOS (CoreGraphics)
  #ifdef __APPLE__
    return std::make_unique<MacCapture>();
  #endif
  
  // No capture backend available
  return nullptr;
}
```

**Important**: Hyprland sets both `HYPRLAND_INSTANCE_SIGNATURE` AND `WAYLAND_DISPLAY`. Detection must check Hyprland first to use its native API instead of generic Wayland protocol.

## Error Handling Strategy

Use `VisionError` enum for all fallible operations:

```cpp
enum class VisionError {
  NoAuth,           // No Gemini API authentication
  CaptureFailed,   // Screen capture failed (permissions, display)
  InvalidImage,    // Image file corrupted or unsupported format
  ImageTooLarge,   // Exceeds size limits after base64 encoding
  ApiError,        // Gemini API returned error
  Timeout,         // API request timed out (30s default)
  PlatformNotSupported // No capture backend for this OS
};

template<typename T>
using Result = std::variant<T, VisionError>;

Result<ImageData> capture_screen() {
  auto backend = ScreenCapture::create_platform_capture();
  if (!backend) {
    return VisionError::PlatformNotSupported;
  }
  
  auto result = backend->capture();
  if (!result.has_value()) {
    return VisionError::CaptureFailed;
  }
  return result.value();
}
```