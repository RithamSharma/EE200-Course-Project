#pragma once

#include <string>
#include <vector>
#include <optional>
#include <chrono>
#include <variant>

namespace novashell::vision {

enum class VisionError {
  NoAuth,
  CaptureFailed,
  InvalidImage,
  ImageTooLarge,
  ApiError,
  Timeout,
  PlatformNotSupported
};

enum class ImageFormat {
  PNG,
  JPEG,
  BMP,
  WebP,
  Unknown
};

struct ImageData {
  int width = 0;
  int height = 0;
  int channels = 4;
  std::vector<uint8_t> data;
  ImageFormat format = ImageFormat::PNG;
  std::chrono::steady_clock::time_point timestamp;

  ImageData() = default;
  ImageData(int w, int h, int c);
  ImageData(const ImageData&) = default;
  ImageData(ImageData&&) noexcept;
  ImageData& operator=(const ImageData&) = default;
  ImageData& operator=(ImageData&&) noexcept;
};

template<typename T>
struct Result {
private:
  std::variant<T, VisionError> data_;

public:
  Result(T value) : data_(value) {}
  Result(VisionError error) : data_(error) {}

  bool ok() const {
    return std::holds_alternative<T>(data_);
  }
  T& value() {
    return std::get<T>(data_);
  }
  const T& value() const {
    return std::get<T>(data_);
  }
  VisionError error() const {
    if (std::holds_alternative<VisionError>(data_)) {
      return std::get<VisionError>(data_);
    }
    return VisionError::NoAuth;
  }
};

using ImageResult = Result<ImageData>;

VisionError get_error(VisionError e);
const char* error_message(VisionError e);

} // namespace novashell::vision