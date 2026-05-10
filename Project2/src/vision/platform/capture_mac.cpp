#include <vision/platform/capture_mac.h>

namespace novashell::vision {

MacCapture::MacCapture() = default;
MacCapture::~MacCapture() = default;

ImageResult MacCapture::capture() {
#if defined(__APPLE__)
  return VisionError::PlatformNotSupported;
#else
  return VisionError::PlatformNotSupported;
#endif
}

ImageResult MacCapture::capture_display(void* display_id) {
  (void)display_id;
  return VisionError::PlatformNotSupported;
}

bool MacCapture::is_supported() const {
#if defined(__APPLE__)
  return true;
#else
  return false;
#endif
}

ImageResult MacCapture::cgimage_to_imagedata(void* img) {
  (void)img;
  return VisionError::PlatformNotSupported;
}

} // namespace novashell::vision