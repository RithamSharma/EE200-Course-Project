#include <vision/types.h>

namespace novashell::vision {

ImageData::ImageData(int w, int h, int c)
    : width(w), height(h), channels(c), data(w * h * c) {}

ImageData::ImageData(ImageData&& other) noexcept
    : width(other.width), height(other.height), channels(other.channels),
      data(std::move(other.data)), format(other.format), timestamp(other.timestamp) {
  other.width = 0;
  other.height = 0;
  other.channels = 0;
}

ImageData& ImageData::operator=(ImageData&& other) noexcept {
  if (this != &other) {
    width = other.width;
    height = other.height;
    channels = other.channels;
    data = std::move(other.data);
    format = other.format;
    timestamp = other.timestamp;
    other.width = 0;
    other.height = 0;
    other.channels = 0;
  }
  return *this;
}

const char* error_message(VisionError e) {
  switch (e) {
    case VisionError::NoAuth: return "No authentication configured. Use 'login' or 'set-key'.";
    case VisionError::CaptureFailed: return "Screen capture failed. Check permissions.";
    case VisionError::InvalidImage: return "Invalid image file or unsupported format.";
    case VisionError::ImageTooLarge: return "Image exceeds maximum size limit.";
    case VisionError::ApiError: return "API error occurred.";
    case VisionError::Timeout: return "Request timed out.";
    case VisionError::PlatformNotSupported: return "Screen capture not supported on this platform.";
  }
  return "Unknown error";
}

} // namespace novashell::vision