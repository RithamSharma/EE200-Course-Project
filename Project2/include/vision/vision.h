#pragma once

#include "types.h"
#include "analysis.h"

namespace novashell::vision {

struct Region {
  int x, y, width, height;
};

class VisionTUI {
public:
  VisionTUI();
  ~VisionTUI();

  void display_image(const ImageData& img);
  Region select_region();
  AnalysisMode select_mode();
  void show_progress(const std::string& message);
  void show_error(const std::string& message);
};

class SeeCommand {
public:
  SeeCommand();
  int run(const std::optional<std::string>& image_path = std::nullopt);

private:
  Result<ImageData> capture_screen();
  Result<ImageData> load_image(const std::string& path);
  Result<ImageData> select_region(ImageData& img);
  AnalysisMode select_mode();
  Result<std::string> analyze(const ImageData& img, AnalysisMode mode);
  void display_result(const std::string& result);
  bool check_authentication();
};

} // namespace novashell::vision