#pragma once

#include "types.h"

namespace novashell::vision {

struct ProcessedImage {
  std::string base64_data;
  std::string mime_type;
  size_t original_size;
  size_t processed_size;
};

class ImageProcessor {
public:
  static ImageResult load_from_file(const std::string& path);
  static Result<ProcessedImage> prepare_for_api(const ImageData& img);
  static ImageResult resize(const ImageData& img, int max_w, int max_h);
  static ImageResult convert_to_png(const ImageData& img);
  static Result<ImageFormat> detect_format(const uint8_t* data, size_t size);
  static std::string to_base64(const ImageData& img);
};

} // namespace novashell::vision