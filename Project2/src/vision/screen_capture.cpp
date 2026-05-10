#include <vision/screen_capture.h>
#include <vision/platform/capture_hyprland.h>
#include <vision/platform/capture_wayland.h>
#include <vision/platform/capture_x11.h>
#include <memory>
#include <cstdlib>

namespace novashell::vision {

std::unique_ptr<ScreenCapture> ScreenCapture::create_platform_capture() {
#if defined(__linux__) || defined(__linux)
  if (const char* hyprland_sig = getenv("HYPRLAND_INSTANCE_SIGNATURE")) {
    (void)hyprland_sig;
    return std::make_unique<HyprlandCapture>();
  }
  if (const char* wayland_display = getenv("WAYLAND_DISPLAY")) {
    (void)wayland_display;
    return std::make_unique<WaylandCapture>();
  }
  if (const char* display = getenv("DISPLAY")) {
    (void)display;
    return std::make_unique<X11Capture>();
  }
#elif defined(_WIN32) || defined(_WIN64)
  return std::make_unique<Win32Capture>();
#elif defined(__APPLE__)
  return std::make_unique<MacCapture>();
#endif
  return nullptr;
}

} // namespace novashell::vision