#include <vision/platform/capture_win32.h>

namespace novashell::vision {

Win32Capture::Win32Capture() = default;
Win32Capture::~Win32Capture() = default;

ImageResult Win32Capture::capture() {
#if defined(_WIN32) || defined(_WIN64)
  return VisionError::PlatformNotSupported;
#else
  return VisionError::PlatformNotSupported;
#endif
}

ImageResult Win32Capture::capture_monitor(int index) {
  (void)index;
  return VisionError::PlatformNotSupported;
}

bool Win32Capture::is_supported() const {
#if defined(_WIN32) || defined(_WIN64)
  return true;
#else
  return false;
#endif
}

} // namespace novashell::vision