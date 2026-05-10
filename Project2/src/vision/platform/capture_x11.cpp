#include <vision/platform/capture_x11.h>

#include <cstdlib>

namespace novashell::vision {

X11Capture::X11Capture() = default;
X11Capture::~X11Capture() = default;

ImageResult X11Capture::capture() {
  return VisionError::PlatformNotSupported;
}

bool X11Capture::is_supported() const {
  return getenv("DISPLAY") != nullptr;
}

} // namespace novashell::vision