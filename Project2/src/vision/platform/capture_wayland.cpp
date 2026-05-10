#include <vision/platform/capture_wayland.h>

#include <cstdlib>

namespace novashell::vision {

WaylandCapture::WaylandCapture() = default;
WaylandCapture::~WaylandCapture() = default;

ImageResult WaylandCapture::capture() {
  return VisionError::PlatformNotSupported;
}

bool WaylandCapture::is_supported() const {
  const char* wayland = getenv("WAYLAND_DISPLAY");
  const char* hyprland = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  return wayland && !hyprland;
}

ImageResult WaylandCapture::capture_via_screencopy() {
  return VisionError::PlatformNotSupported;
}

ImageResult WaylandCapture::capture_via_gnome_screenshot() {
  return VisionError::PlatformNotSupported;
}

} // namespace novashell::vision