# Task 011: Implement SeeCommand orchestrator

**depends-on**: task-005, task-006, task-007, task-008, task-009, task-010

## Description

Implement the SeeCommand class that orchestrates the complete vision workflow: capture → process → analyze → display.

## Execution Context

**Task Number**: 11 of 16
**Phase**: Core Features
**Prerequisites**: All TUI and platform capture tasks (005-010) must be complete

## BDD Scenario

```gherkin
Scenario: Full Screen Capture and Analysis
  Given NovaShell is running with valid Gemini API authentication
  When the user types `see` without arguments
  Then the system captures the entire screen
  And opens an interactive TUI showing the captured image
  And prompts the user to select analysis mode
  And sends the image to Gemini for analysis
  And displays the AI response in the terminal
```

```gherkin
Scenario: Image File Analysis
  Given NovaShell is running with valid Gemini API authentication
  When the user types `see /path/to/image.png`
  Then the system validates the file exists and is an image
  And loads and processes the image
  And proceeds to analysis mode selection
  And sends the image to Gemini for analysis
```

```gherkin
Scenario: No Authentication Handling
  Given NovaShell is running without valid Gemini API authentication
  When the user types `see`
  Then the system displays: "Vision requires Gemini API authentication. Use 'login' or 'set-key' to configure."
  And returns to shell prompt
```

```gherkin
Scenario: Capture Failure Handling
  Given screen capture is initiated
  When the platform capture fails (e.g., permissions denied)
  Then the system displays platform-specific error message
  And suggests remediation steps
  And returns to shell prompt
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Vision Screen Analysis section

## Files to Modify/Create

- Create: `include/vision/vision.h`
- Create: `src/vision/vision.cpp`

## Steps

### Step 1: Define SeeCommand class
- Create class with public `run()` method
- Handle optional image path argument
- Orchestrate the capture → TUI → analyze flow

### Step 2: Implement screen capture flow
- Create platform capture via factory
- Check for capture support
- Handle capture failures gracefully

### Step 3: Implement file loading flow
- Validate file exists and is readable
- Check magic bytes for format validation
- Load via ImageProcessor::load_from_file()

### Step 4: Implement TUI integration
- Display captured/loaded image
- Allow region selection
- Get analysis mode from user

### Step 5: Implement analysis submission
- Build multimodal prompt for Gemini
- Send via call_gemini_vision() (extends existing API)
- Handle response and display

### Step 6: Implement error handling
- Check authentication before proceeding
- Display appropriate error messages
- Clean up resources on exit

## Interface Signatures

```cpp
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
```

## Verification Commands

```bash
# Compile SeeCommand
g++ -std=c++20 -c src/vision/vision.cpp -I include

# Integration test
./novashell see
./novashell see /path/to/image.png
./novashell see (without auth - should show error)
```

## Success Criteria

- `see` captures screen and shows TUI
- `see /path` loads and analyzes image file
- No auth shows appropriate error
- Capture failure shows platform-specific error
- All error conditions handled gracefully