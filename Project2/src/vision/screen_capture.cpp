#include <vision/screen_capture.h>
#include <memory>
#include <cstdlib>

namespace novashell::vision {

std::unique_ptr<ScreenCapture> ScreenCapture::create_platform_capture() {
#if defined(__linux__) || defined(__linux)
  if (const char* hyprland_sig = getenv("HYPRLAND_INSTANCE_SIGNATURE")) {
    (void)hyprland_sig;
    // TODO: Create HyprlandCapture
    return nullptr;
  }
  if (const char* wayland_display = getenv("WAYLAND_DISPLAY")) {
    (void)wayland_display;
    // TODO: Create WaylandCapture
    return nullptr;
  }
  if (const char* display = getenv("DISPLAY")) {
    (void)display;
    // TODO: Create X11Capture
    return nullptr;
  }
#elif defined(_WIN32) || defined(_WIN64)
  // TODO: Create Win32Capture
  return nullptr;
#elif defined(__APPLE__)
  // TODO: Create MacCapture
  return nullptr;
#endif
  return nullptr;
}

} // namespace novashell::vision