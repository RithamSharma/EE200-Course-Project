#include <vision/vision.h>
#include <vision/screen_capture.h>
#include <vision/image_utils.h>
#include <iostream>

namespace novashell::vision {

VisionTUI::VisionTUI() = default;
VisionTUI::~VisionTUI() = default;

void VisionTUI::display_image(const ImageData& img) {
  (void)img;
  std::cout << "[Vision TUI] Display image " << img.width << "x" << img.height << std::endl;
}

Region VisionTUI::select_region() {
  return {0, 0, 800, 600};
}

AnalysisMode VisionTUI::select_mode() {
  return AnalysisMode::ScreenUnderstanding;
}

void VisionTUI::show_progress(const std::string& message) {
  std::cout << "[Vision TUI] " << message << std::endl;
}

void VisionTUI::show_error(const std::string& message) {
  std::cerr << "[Vision TUI] Error: " << message << std::endl;
}

SeeCommand::SeeCommand() = default;

int SeeCommand::run(const std::optional<std::string>& image_path) {
  if (!check_authentication()) {
    std::cerr << "Vision requires API authentication. Set GEMINI_API_KEY or OPENAI_API_KEY environment variable." << std::endl;
    return 1;
  }

  ImageData img;
  if (image_path) {
    auto result = load_image(*image_path);
    if (!result.ok()) {
      std::cerr << "Error: " << error_message(result.error()) << std::endl;
      return 1;
    }
    img = result.value();
    std::cout << "Image loaded: " << img.width << "x" << img.height << std::endl;
  } else {
    std::cout << "Capturing screen..." << std::endl;
    auto result = capture_screen();
    if (!result.ok()) {
      std::cerr << "Error: " << error_message(result.error()) << std::endl;
      return 1;
    }
    img = result.value();
    std::cout << "Screen captured: " << img.width << "x" << img.height << std::endl;
  }

  // Select analysis mode
  AnalysisMode mode = select_mode();
  std::cout << "Analyzing with " << (mode == AnalysisMode::ScreenUnderstanding ? "Screen Understanding" :
                                     mode == AnalysisMode::OcrTextExtraction ? "OCR" :
                                     mode == AnalysisMode::AccessibilityAudit ? "Accessibility Audit" : "Full Analysis") << "..." << std::endl;

  // Send to AI for analysis
  std::cout << "Sending to OpenAI for analysis..." << std::endl;
  auto analysis_result = analyze(img, mode);
  if (!analysis_result.ok()) {
    std::cerr << "Analysis failed: API error occurred" << std::endl;
    return 1;
  }

  std::cout << "\n--- AI Response ---\n" << analysis_result.value() << "\n--- End ---\n";

  return 0;
}

Result<ImageData> SeeCommand::capture_screen() {
  auto capture = ScreenCapture::create_platform_capture();
  if (!capture) {
    return VisionError::PlatformNotSupported;
  }
  if (!capture->is_supported()) {
    return VisionError::CaptureFailed;
  }
  return capture->capture();
}

Result<ImageData> SeeCommand::load_image(const std::string& path) {
  return ImageProcessor::load_from_file(path);
}

Result<ImageData> SeeCommand::select_region(ImageData& img) {
  VisionTUI tui;
  tui.display_image(img);
  Region region = tui.select_region();

  ImageData cropped;
  cropped.width = region.width;
  cropped.height = region.height;
  cropped.channels = img.channels;
  cropped.data.resize(region.width * region.height * img.channels);

  for (int y = 0; y < region.height && (y + region.y) < img.height; ++y) {
    for (int x = 0; x < region.width && (x + region.x) < img.width; ++x) {
      for (int c = 0; c < img.channels; ++c) {
        cropped.data[(y * region.width + x) * img.channels + c] =
            img.data[((region.y + y) * img.width + region.x + x) * img.channels + c];
      }
    }
  }

  return cropped;
}

AnalysisMode SeeCommand::select_mode() {
  VisionTUI tui;
  return tui.select_mode();
}

Result<std::string> SeeCommand::analyze(const ImageData& img, AnalysisMode mode) {
  AnalysisEngine engine;
  return engine.analyze(img, mode);
}

void SeeCommand::display_result(const std::string& result) {
  std::cout << result << std::endl;
}

bool SeeCommand::check_authentication() {
  const char* gemini_key = getenv("GEMINI_API_KEY");
  const char* openai_key = getenv("OPENAI_API_KEY");
  return gemini_key != nullptr || openai_key != nullptr;
}

} // namespace novashell::vision