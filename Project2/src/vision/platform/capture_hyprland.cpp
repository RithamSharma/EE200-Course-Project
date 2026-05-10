#include <vision/platform/capture_hyprland.h>

#include <cstdlib>
#include <cstdio>
#include <fstream>

namespace novashell::vision {

HyprlandCapture::HyprlandCapture() = default;
HyprlandCapture::~HyprlandCapture() = default;

ImageResult HyprlandCapture::capture() {
  // Try grim first (most reliable on Hyprland)
  auto result = capture_via_grim();
  if (result.ok()) {
    return result;
  }
  // Fall back to hyprctl
  return capture_via_hyprctl();
}

bool HyprlandCapture::is_supported() const {
  return getenv("HYPRLAND_INSTANCE_SIGNATURE") != nullptr;
}

ImageResult HyprlandCapture::capture_via_grim() {
  // Use grim to capture screen to temp file
  const char* tmpdir = getenv("TMPDIR") ? getenv("TMPDIR") : "/tmp";
  std::string filepath = std::string(tmpdir) + "/nova_capture.png";

  std::string cmd = "grim " + filepath;

  int ret = system(cmd.c_str());
  if (ret != 0) {
    return VisionError::CaptureFailed;
  }

  // Load the PNG file using file command
  FILE* f = fopen(filepath.c_str(), "rb");
  if (!f) {
    return VisionError::InvalidImage;
  }

  // Get file size
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  // Read PNG file
  std::vector<uint8_t> buffer(size);
  size_t read = fread(buffer.data(), 1, size, f);
  fclose(f);

  if (read != static_cast<size_t>(size)) {
    return VisionError::InvalidImage;
  }

  // Clean up temp file
  std::remove(filepath.c_str());

  // Store PNG data directly (will be parsed later)
  ImageData img;
  img.format = ImageFormat::PNG;
  img.data = std::move(buffer);
  img.width = 1920;
  img.height = 1080;
  img.channels = 4;
  img.timestamp = std::chrono::steady_clock::now();

  return img;
}

ImageResult HyprlandCapture::capture_via_hyprctl() {
  // Alternative: use hyprctl to get screenshot
  std::string cmd = "hyprctl -j screenshot";

  FILE* pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    return VisionError::CaptureFailed;
  }

  char buffer[4096];
  std::string result;
  while (fgets(buffer, sizeof(buffer), pipe)) {
    result += buffer;
  }
  pclose(pipe);

  // Parse JSON response and decode base64 image
  // This is a simplified fallback
  return VisionError::CaptureFailed;
}

std::string HyprlandCapture::get_hyprsocket_path() {
  const char* hyprsocket = getenv("HYPRLAND_SOCKET_PATH");
  if (hyprsocket) {
    return hyprsocket;
  }
  return "";
}

} // namespace novashell::vision