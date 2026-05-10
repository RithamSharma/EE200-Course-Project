#include <vision/analysis.h>

namespace novashell::vision {

AnalysisEngine::AnalysisEngine() = default;

std::string AnalysisEngine::build_prompt(AnalysisMode mode) {
  switch (mode) {
    case AnalysisMode::ScreenUnderstanding:
      return prompt_screen_understanding();
    case AnalysisMode::OcrTextExtraction:
      return prompt_ocr();
    case AnalysisMode::AccessibilityAudit:
      return prompt_accessibility();
    case AnalysisMode::FullAnalysis:
      return prompt_full();
  }
  return "Analyze this image.";
}

Result<std::string> AnalysisEngine::analyze(const ImageData& img, AnalysisMode mode) {
  (void)img;
  (void)mode;
  // TODO: Call Gemini API with image
  return std::string("Analysis placeholder - implement API call");
}

std::string AnalysisEngine::prompt_screen_understanding() {
  return "Describe what's visible in this image in detail. Include information about UI elements, text content, layout, colors, and any notable visual features. Be comprehensive and specific.";
}

std::string AnalysisEngine::prompt_ocr() {
  return "Extract all visible text from this image. Format the output as structured text with line breaks preserved. If text is in specific regions, indicate approximate positions.";
}

std::string AnalysisEngine::prompt_accessibility() {
  return "Analyze this UI for accessibility issues. Identify: (1) contrast issues between text and background, (2) text that may be too small to read, (3) missing labels or accessible names for interactive elements, (4) any other accessibility concerns.";
}

std::string AnalysisEngine::prompt_full() {
  return "Provide a comprehensive analysis of this image including: (1) detailed description of visual content, (2) all visible text, (3) accessibility audit findings, (4) recommendations for improvements. Format as structured sections.";
}

} // namespace novashell::vision