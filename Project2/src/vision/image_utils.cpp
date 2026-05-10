#include <vision/image_utils.h>
#include <fstream>
#include <cstring>

namespace novashell::vision {

ImageResult ImageProcessor::load_from_file(const std::string& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    return VisionError::InvalidImage;
  }

  auto size = file.tellg();
  if (size <= 0 || static_cast<size_t>(size) > 10 * 1024 * 1024) {
    return VisionError::ImageTooLarge;
  }

  file.seekg(0);
  std::vector<uint8_t> buffer(size);
  if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
    return VisionError::InvalidImage;
  }

  auto format = detect_format(buffer.data(), buffer.size());
  if (!format.ok()) {
    return format.error();
  }

  ImageData img;
  img.format = format.value();
  img.width = static_cast<int>(size);
  img.height = 1;
  img.channels = 3;
  img.data = std::move(buffer);
  img.timestamp = std::chrono::steady_clock::now();

  return img;
}

Result<ProcessedImage> ImageProcessor::prepare_for_api(const ImageData& img) {
  if (img.data.empty()) {
    return VisionError::InvalidImage;
  }

  ProcessedImage result;
  result.base64_data = to_base64(img);
  result.original_size = img.data.size();
  result.processed_size = result.base64_data.size();
  result.mime_type = "image/png";

  return result;
}

ImageResult ImageProcessor::resize(const ImageData& img, int max_w, int max_h) {
  ImageData result = img;

  float scale_w = (max_w > 0) ? static_cast<float>(max_w) / img.width : 1.0f;
  float scale_h = (max_h > 0) ? static_cast<float>(max_h) / img.height : 1.0f;
  float scale = std::min(scale_w, scale_h);

  if (scale >= 1.0f) {
    return result;
  }

  result.width = static_cast<int>(img.width * scale);
  result.height = static_cast<int>(img.height * scale);
  result.data.resize(result.width * result.height * result.channels);

  // Simple nearest-neighbor resize
  for (int y = 0; y < result.height; ++y) {
    for (int x = 0; x < result.width; ++x) {
      int src_x = x / scale;
      int src_y = y / scale;
      src_x = std::min(src_x, img.width - 1);
      src_y = std::min(src_y, img.height - 1);

      for (int c = 0; c < img.channels; ++c) {
        result.data[(y * result.width + x) * result.channels + c] =
            img.data[(src_y * img.width + src_x) * img.channels + c];
      }
    }
  }

  return result;
}

ImageResult ImageProcessor::convert_to_png(const ImageData& img) {
  ImageData result = img;
  result.format = ImageFormat::PNG;
  // In real implementation, would convert format here
  return result;
}

Result<ImageFormat> ImageProcessor::detect_format(const uint8_t* data, size_t size) {
  if (size < 12) {
    return VisionError::InvalidImage;
  }

  // PNG magic: 89 50 4E 47
  if (data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47) {
    return ImageFormat::PNG;
  }
  // JPEG magic: FF D8
  if (data[0] == 0xFF && data[1] == 0xD8) {
    return ImageFormat::JPEG;
  }
  // BMP magic: 42 4D (BM)
  if (data[0] == 0x42 && data[1] == 0x4D) {
    return ImageFormat::BMP;
  }
  // WebP magic: RIFF....WEBP
  if (data[0] == 0x52 && data[1] == 0x49 && data[2] == 0x46 && data[3] == 0x46 &&
      size >= 12 && data[8] == 0x57 && data[9] == 0x45 && data[10] == 0x42 && data[11] == 0x50) {
    return ImageFormat::WebP;
  }

  return ImageFormat::Unknown;
}

std::string ImageProcessor::to_base64(const ImageData& img) {
  static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  std::string result;
  size_t padding = (3 - (img.data.size() % 3)) % 3;

  for (size_t i = 0; i < img.data.size(); i += 3) {
    uint8_t b0 = img.data[i];
    uint8_t b1 = (i + 1 < img.data.size()) ? img.data[i + 1] : 0;
    uint8_t b2 = (i + 2 < img.data.size()) ? img.data[i + 2] : 0;

    result += chars[b0 >> 2];
    result += chars[((b0 & 0x03) << 4) | (b1 >> 4)];
    result += chars[((b1 & 0x0F) << 2) | (b2 >> 6)];
    result += chars[b2 & 0x3F];
  }

  for (size_t i = 0; i < padding; ++i) {
    result[result.size() - 1 - i] = '=';
  }

  return result;
}

} // namespace novashell::vision