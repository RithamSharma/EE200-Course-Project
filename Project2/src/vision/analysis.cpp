#include <vision/analysis.h>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>

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

static std::string read_file(const std::string& path) {
  std::ifstream f(path);
  std::ostringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

Result<std::string> AnalysisEngine::analyze(const ImageData& img, AnalysisMode mode) {
  const char* openai_key = getenv("OPENAI_API_KEY");
  if (!openai_key) {
    return std::string("Error: OpenAI API key not found. Set OPENAI_API_KEY environment variable.");
  }

  std::string prompt = build_prompt(mode);
  std::string base64_data;

  // Convert image to base64 (simple version for PNG)
  for (size_t i = 0; i < img.data.size(); i += 3) {
    uint8_t b0 = img.data[i];
    uint8_t b1 = (i + 1 < img.data.size()) ? img.data[i + 1] : 0;
    uint8_t b2 = (i + 2 < img.data.size()) ? img.data[i + 2] : 0;
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    base64_data += chars[b0 >> 2];
    base64_data += chars[((b0 & 0x03) << 4) | (b1 >> 4)];
    base64_data += (i + 1 < img.data.size()) ? chars[((b1 & 0x0F) << 2) | (b2 >> 6)] : '=';
    base64_data += (i + 2 < img.data.size()) ? chars[b2 & 0x3F] : '=';
  }

  // Create JSON payload for OpenAI vision API
  std::string json_payload = R"({
    "model": "gpt-4o",
    "messages": [
      {
        "role": "user",
        "content": [
          {"type": "text", "text": ")" + prompt + R"("},
          {"type": "image_url", "image_url": {"url": "data:image/png;base64,)" + base64_data + R"("}}
        ]
      }
    ],
    "max_tokens": 4096
  })";

  // Write payload to temp file
  std::ofstream json_file("/tmp/vision_payload.json");
  json_file << json_payload;
  json_file.close();

  // Call OpenAI API
  std::string cmd = "curl -s -X POST https://api.openai.com/v1/chat/completions "
                    "-H 'Authorization: Bearer " + std::string(openai_key) + "' "
                    "-H 'Content-Type: application/json' "
                    "-d @/tmp/vision_payload.json";

  FILE* pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    return std::string("Error: Failed to execute curl");
  }

  char buffer[4096];
  std::string result;
  while (fgets(buffer, sizeof(buffer), pipe)) {
    result += buffer;
  }
  pclose(pipe);

  // Parse response (simple extraction)
  if (result.find("\"error\"") != std::string::npos) {
    size_t msg_start = result.find("\"message\":\"");
    if (msg_start != std::string::npos) {
      msg_start += 11;
      size_t msg_end = result.find("\"", msg_start);
      return std::string("OpenAI Error: ") + result.substr(msg_start, msg_end - msg_start);
    }
    return std::string("OpenAI API error: ") + result;
  }

  // Extract content from response
  size_t content_start = result.find("\"content\":\"");
  if (content_start != std::string::npos) {
    content_start += 11;
    size_t content_end = result.find("\"", content_start);
    std::string content = result.substr(content_start, content_end - content_start);
    // Unescape newlines
    for (size_t pos = content.find("\\n"); pos != std::string::npos; pos = content.find("\\n")) {
      content.replace(pos, 2, "\n");
    }
    return content;
  }

  return result;
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