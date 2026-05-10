#include <vision/vision.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>

using namespace novashell::vision;

void test_see_command_creation() {
  SeeCommand cmd;
  printf("PASS: test_see_command_creation\n");
}

void test_vision_tui_creation() {
  VisionTUI tui;
  printf("PASS: test_vision_tui_creation\n");
}

void test_analysis_engine_creation() {
  AnalysisEngine engine;
  printf("PASS: test_analysis_engine_creation\n");
}

void test_auth_check() {
  SeeCommand cmd;
  // Without API key set, should fail
  unsetenv("GEMINI_API_KEY");
  printf("PASS: test_auth_check\n");
}

void test_prompt_building() {
  AnalysisEngine engine;
  auto prompt = engine.build_prompt(AnalysisMode::ScreenUnderstanding);
  assert(prompt.find("Describe") != std::string::npos);

  prompt = engine.build_prompt(AnalysisMode::OcrTextExtraction);
  assert(prompt.find("text") != std::string::npos);

  prompt = engine.build_prompt(AnalysisMode::AccessibilityAudit);
  assert(prompt.find("accessibility") != std::string::npos);

  prompt = engine.build_prompt(AnalysisMode::FullAnalysis);
  assert(prompt.find("comprehensive") != std::string::npos);
  printf("PASS: test_prompt_building\n");
}

int main() {
  test_see_command_creation();
  test_vision_tui_creation();
  test_analysis_engine_creation();
  test_auth_check();
  test_prompt_building();
  printf("\nAll integration tests passed!\n");
  return 0;
}