#include <vision/platform/capture_hyprland.h>

#include <cstdlib>

namespace novashell::vision {

HyprlandCapture::HyprlandCapture() = default;
HyprlandCapture::~HyprlandCapture() = default;

ImageResult HyprlandCapture::capture() {
  return VisionError::PlatformNotSupported;
}

bool HyprlandCapture::is_supported() const {
  return getenv("HYPRLAND_INSTANCE_SIGNATURE") != nullptr;
}

ImageResult HyprlandCapture::capture_via_hyprctl() {
  return VisionError::PlatformNotSupported;
}

ImageResult HyprlandCapture::capture_via_grim() {
  return VisionError::PlatformNotSupported;
}

std::string HyprlandCapture::get_hyprsocket_path() {
  return "";
}

} // namespace novashell::vision