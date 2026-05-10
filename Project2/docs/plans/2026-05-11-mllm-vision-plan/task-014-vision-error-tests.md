# Task 014: Add vision error scenarios tests

**depends-on**: task-004

## Description

Add unit tests for error handling scenarios: invalid images, large image handling, API errors, and auth validation.

## Execution Context

**Task Number**: 14 of 16
**Phase**: Testing
**Prerequisites**: Task 004 (ImageProcessor) should be complete for testing image validation

## BDD Scenario

```gherkin
Scenario: Invalid Image File Handling
  Given NovaShell is running with valid Gemini API authentication
  When the user types `see /path/to/invalid.file`
  Then the system displays error: "Invalid image file or unsupported format"
  And lists supported formats: PNG, JPEG, WebP, BMP
  And returns to shell prompt
```

```gherkin
Scenario: Large Image Rejection
  Given the user provides an image file larger than 8MB
  When the image is being processed
  Then calculate estimated base64 size (file × 1.37)
  And if exceeds 5MB, resize to maximum 1920x1080 before encoding
  And show progress indicator during resize
  And proceed to analysis with optimized image
```

```gherkin
Scenario: API Timeout Handling
  Given vision analysis is in progress
  When the API response takes longer than 30 seconds
  Then display timeout error message
  And preserve captured image for manual retry
  And suggest checking network connection
```

```gherkin
Scenario: Auth Token Expiry
  Given NovaShell has valid authentication
  When auth token expires mid-session
  Then detect 401 response from API
  And display: "Session expired. Please run 'login' or 'set-key' to re-authenticate."
  And return to shell prompt without crashing
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Error Handling section

## Files to Create

- Create: `tests/vision/error_handling_test.cpp`

## Steps

### Step 1: Set up test framework
- Use existing test pattern from project (or create with doctest)
- Create test fixtures for VisionError scenarios

### Step 2: Test invalid image handling
- Create test with non-image file
- Verify VisionError::InvalidImage is returned
- Verify error message includes supported formats

### Step 3: Test large image handling
- Create mock large image data
- Verify resize is triggered
- Verify aspect ratio is preserved

### Step 4: Test auth validation
- Mock authentication state
- Test that missing auth returns VisionError::NoAuth

### Step 5: Test API error propagation
- Mock API failures (timeout, 401, 429, etc.)
- Verify correct VisionError enum values

## Verification Commands

```bash
# Compile and run tests
g++ -std=c++20 -c tests/vision/error_handling_test.cpp -I include -I tests
./build/vision_tests --test error_handling
```

## Success Criteria

- All error scenarios have test coverage
- Tests use test doubles for external dependencies
- VisionError enum values are correctly mapped
- Error messages are user-friendly