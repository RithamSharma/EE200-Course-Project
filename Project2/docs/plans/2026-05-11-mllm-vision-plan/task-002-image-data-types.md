# Task 002: Implement ImageData and VisionError types

**depends-on**: task-001

## Description

Define the core data types for the vision module: `ImageData` struct for holding image buffers and metadata, and `VisionError` enum for error classification.

## Execution Context

**Task Number**: 2 of 16
**Phase**: Foundation
**Prerequisites**: Task 001 (project structure) must be complete

## BDD Scenario

```gherkin
Scenario: Vision error handling framework
  Given NovaShell has the VisionError enum defined
  When a vision operation fails
  Then the system returns a specific VisionError value
  And error messages provide actionable remediation steps
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Error Handling section

## Files to Modify/Create

- Create: `include/vision/types.h`

## Steps

### Step 1: Define VisionError enum
- Create enum with values: `NoAuth`, `CaptureFailed`, `InvalidImage`, `ImageTooLarge`, `ApiError`, `Timeout`, `PlatformNotSupported`
- Add error message mapping function

### Step 2: Define ImageData struct
- Create struct with fields:
  - `width`, `height` (int)
  - `channels` (int, typically 4 for RGBA)
  - `data` (std::vector<uint8_t>)
  - `format` (enum: PNG, JPEG, BMP, WebP)
  - `timestamp` (std::chrono::steady_clock::time_point)

### Step 3: Define Result type alias
- Create `Result<T>` using std::variant<T, VisionError>
- Add helper functions for checking success/failure

### Step 4: Add move semantics support
- Ensure ImageData supports move operations for efficient transfer

## Verification Commands

```bash
# Compile types test
g++ -std=c++20 -c include/vision/types.h -o /tmp/types_test.o
```

## Success Criteria

- VisionError enum has all required values
- ImageData struct has all required fields
- Result type alias works correctly
- Code compiles without errors