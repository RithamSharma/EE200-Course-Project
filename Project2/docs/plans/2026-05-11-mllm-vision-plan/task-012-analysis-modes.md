# Task 012: Implement analysis prompt crafting and mode selection

**depends-on**: task-004

## Description

Implement the analysis engine that crafts appropriate prompts for each analysis mode and formats the results.

## Execution Context

**Task Number**: 12 of 16
**Phase**: Core Features
**Prerequisites**: Task 004 (ImageProcessor) should be complete for prompt building context

## BDD Scenario

```gherkin
Scenario: Screen Understanding Analysis
  Given image is selected
  When user selects "Screen Understanding" mode
  Then send prompt: "Describe what's visible in this image in detail..."
  And request comprehensive description of UI elements, text, layout
```

```gherkin
Scenario: OCR Text Extraction
  Given image is selected
  When user selects "OCR Text Extraction" mode
  Then send prompt: "Extract all visible text from this image..."
  And format output as structured text with positions
```

```gherkin
Scenario: Accessibility Audit
  Given image is selected
  When user selects "Accessibility Audit" mode
  Then send prompt: "Analyze this UI for accessibility issues..."
  And identify contrast issues, small text, missing labels
```

```gherkin
Scenario: Full Analysis
  Given image is selected
  When user selects "Full Analysis" mode
  Then send prompt combining all analysis types
  And include screen understanding, OCR, accessibility, and recommendations
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Analysis Modes section

## Files to Modify/Create

- Create: `include/vision/analysis.h`
- Create: `src/vision/analysis.cpp`

## Steps

### Step 1: Define AnalysisMode enum
- Create enum with values: ScreenUnderstanding, OcrTextExtraction, AccessibilityAudit, FullAnalysis

### Step 2: Implement prompt crafting
- Create method to generate system prompt for each mode
- Include context about what information to extract
- Add formatting instructions for output

### Step 3: Implement multimodal API call
- Extend existing call_gemini_api() with image support
- Include base64 image data in request
- Handle vision-specific response parsing

### Step 4: Implement result formatting
- Parse API response
- Format output for terminal display
- Handle multi-line responses

## Interface Signatures

```cpp
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
```

## Verification Commands

```bash
# Compile analysis engine
g++ -std=c++20 -c src/vision/analysis.cpp -I include

# Test prompt generation
./build/vision_tests --test prompts

# Integration test
./novashell see  # Select each mode and verify prompts
```

## Success Criteria

- All 4 analysis modes have distinct, descriptive prompts
- API call includes image data correctly
- Response is formatted for terminal display
- Each mode extracts relevant information