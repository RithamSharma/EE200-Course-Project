# MLLM Vision Module - Best Practices

## Security

### Image Data Handling
- **Validate image format**: Check magic bytes before processing (PNG: 89 50 4E 47)
- **Limit file sizes**: Maximum 10MB for uploaded images, ~8MB for captures
- **Sanitize paths**: Prevent directory traversal in `see /path/to/image`
- **Temp file cleanup**: Remove temporary image files after processing

### API Security
- **Preserve existing auth**: Use existing `access_token` / API key handling
- **No logging sensitive data**: Don't log base64 image data or API responses
- **Rate limiting**: Respect Gemini API limits (15 req/min for free tier)

### Platform Permissions
- **X11**: Check DISPLAY env var; warn about xwd permissions
- **Wayland**: Handle screencopy permission denial gracefully
- **macOS**: Request screen recording permission via system dialog
- **Windows**: Handle UAC elevation if needed for capture

## Performance

### Image Processing
- **Lazy loading**: Don't load image until user confirms analysis
- **Progressive resize**: Scale down large images in stages
- **Memory pooling**: Reuse buffer allocations for multiple captures
- **PNG optimization**: Use pngquant or similar for smaller payloads

### API Optimization
- **Max dimensions**: 1920x1080 - balances quality vs API limits
- **Target payload**: Keep base64 under 4MB for faster transmission
- **Compression**: Use quality=85 for JPEG fallback
- **Timeout handling**: 30 second timeout for large images

### Caching
- **No image caching**: Fresh capture each time ensures accuracy
- **Cache auth**: Reuse existing token validation

## Code Quality

### Error Handling
```cpp
enum class VisionError {
  NoAuth,
  CaptureFailed,
  InvalidImage,
  ApiError,
  Timeout,
  UnsupportedFormat
};
```

- Use Result<T, VisionError> pattern for all fallible operations
- Provide actionable error messages with remediation steps
- Log errors silently (no sensitive data) for debugging

### Platform Detection
```cpp
// Auto-detect at startup, allow override
std::unique_ptr<ScreenCapture> ScreenCapture::create_platform_capture() {
  if (getenv("WAYLAND_DISPLAY"))
    return std::make_unique<WaylandCapture>();
  if (getenv("HYPRLAND_INSTANCE_SIGNATURE"))
    return std::make_unique<HyprlandCapture>();
  if (getenv("DISPLAY"))
    return std::make_unique<X11Capture>();
  // ... Windows/macOS detection
}
```

### Cross-Platform Considerations
- **Unicode paths**: Handle UTF-8 filenames on all platforms
- **Line endings**: Use LF only in code, handle CRLF in file paths
- **Shared abstractions**: Platform-specific code isolated in `platform/` folder

### Testing Strategy
- **Unit tests**: ImageProcessor (base64, resize, format conversion)
- **Integration tests**: Full capture → analyze flow (mock API)
- **Platform tests**: Manual testing on each OS with CI/CD where possible

## Code Style

### Naming Conventions
```cpp
// Classes: PascalCase
class SeeCommand {};
class ImageProcessor {};

// Methods: snake_case
void display_result(const std::string& result);

// Members: trailing underscore
int width_;
std::vector<uint8_t> buffer_;

// Constants: UPPER_SNAKE
constexpr int MAX_IMAGE_SIZE = 10 * 1024 * 1024;
```

### Header Organization
```cpp
#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace novashell::vision {

// Forward declarations
class ScreenCapture;
struct ImageData;
enum class AnalysisMode;

// Main interface
class SeeCommand {
public:
  // ...
};

} // namespace novashell::vision
```

## Dependencies

### Required (existing)
- curl (for API calls) - already in project
- libreadline (for shell input) - already in project

### New Dependencies
- **Linux X11**: libx11, libxext (if not already present)
- **Linux Wayland**: libwayland-client (optional, fallback available)
- **Linux Hyprland**: None (uses existing grim or internal API)
- **Windows**: Gdi32 (built-in Windows SDK)
- **macOS**: CoreGraphics (built-in)

### Optional Enhancement
- **stb_image**: Header-only image loading for format detection (single file, public domain)
- **nlohmann/json**: Lighter JSON parsing for API responses

## Documentation

### User-Facing Docs
- `help see` - Command usage and examples
- Error messages with links to troubleshooting
- Platform-specific permission setup instructions

### Developer Docs
- Platform implementation guide in `platform/README.md`
- API integration notes in `vision.cpp` comments
- Debug flags for development: `NOVA_VISION_DEBUG=1`