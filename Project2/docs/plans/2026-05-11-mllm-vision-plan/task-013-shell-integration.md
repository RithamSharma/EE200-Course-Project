# Task 013: Integrate 'see' command into Shell and add auth validation

**depends-on**: task-011

## Description

Integrate the SeeCommand into the existing Shell class and add authentication validation to prevent unauthorized usage.

## Execution Context

**Task Number**: 13 of 16
**Phase**: Integration
**Prerequisites**: Task 011 (SeeCommand) must be complete

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

## Files to Modify/Create

- Modify: `src/shell.cpp` - Add see command handling
- Modify: `src/shell.h` - Add SeeCommand include and member
- Modify: `src/shell.cpp:handle_builtin()` - Add 'see' case
- Modify: `src/shell.cpp:load_config()` - Add help text for 'see' command

## Steps

### Step 1: Add SeeCommand to Shell
- Include `vision/vision.h` in shell.cpp
- Add `SeeCommand vision_command_` member to Shell class
- Initialize in Shell constructor

### Step 2: Add 'see' command handling
- In `handle_builtin()`, add case for "see"
- Parse optional image path argument
- Call `vision_command_.run()`

### Step 3: Add authentication check
- In SeeCommand::run(), check for valid API auth
- Return error message if not authenticated
- Preserve existing auth validation patterns

### Step 4: Update help text
- Add 'see' command description to help output
- Document usage: `see` and `see /path/to/image.png`

## Interface Signatures

```cpp
// In shell.h
#include "vision/vision.h"

class Shell {
  // ... existing members ...
private:
  SeeCommand vision_command_;
  
  // Add method
  void vision_command(const std::string& args);
};
```

## Verification Commands

```bash
# Compile shell with vision integration
make clean && make

# Test integration
./novashell
see                    # Should show auth error if not logged in
see /tmp/test.png      # Should show error if file doesn't exist
set-key YOUR_KEY       # Set API key
see                    # Should work with auth
```

## Success Criteria

- Shell compiles with vision module
- 'see' command recognized by handle_builtin()
- Authentication error shown when not logged in
- Vision feature works after authentication