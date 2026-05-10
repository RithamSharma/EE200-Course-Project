#pragma once

#include "types.h"

namespace novashell::vision {

enum class AnalysisMode {
  ScreenUnderstanding,
  OcrTextExtraction,
  AccessibilityAudit,
  FullAnalysis
};

class AnalysisEngine {
public:
  AnalysisEngine();

  std::string build_prompt(AnalysisMode mode);
  Result<std::string> analyze(const ImageData& img, AnalysisMode mode);

private:
  std::string prompt_screen_understanding();
  std::string prompt_ocr();
  std::string prompt_accessibility();
  std::string prompt_full();
};

} // namespace novashell::vision