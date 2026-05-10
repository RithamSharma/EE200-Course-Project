# Task 015: Add screen capture integration tests

**depends-on**: task-013

## Description

Add integration tests for the complete vision workflow from capture to analysis result.

## Execution Context

**Task Number**: 15 of 16
**Phase**: Testing
**Prerequisites**: Task 013 (Shell integration) should be complete

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

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Vision Screen Analysis section

## Files to Create

- Create: `tests/vision/integration_test.cpp`

## Steps

### Step 1: Set up integration test framework
- Create test harness that mocks terminal input
- Mock Gemini API responses

### Step 2: Test full workflow with mock capture
- Mock ScreenCapture to return known test image
- Verify SeeCommand processes image correctly

### Step 3: Test TUI interaction flow
- Mock keyboard input for region selection
- Verify correct coordinates are passed to analysis

### Step 4: Test analysis mode selection
- Mock each analysis mode selection
- Verify correct prompts are sent

### Step 5: Test result display
- Mock API response
- Verify formatted output is correct

## Verification Commands

```bash
# Compile and run integration tests
g++ -std=c++20 -c tests/vision/integration_test.cpp -I include -I tests -DMOCK_API
./build/vision_tests --test integration
```

## Success Criteria

- Full workflow tests pass with mocked dependencies
- Each analysis mode is tested
- TUI interaction is tested with controlled input
- Result formatting is verified