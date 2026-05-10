# Task 016: Update Makefile with vision module build rules

**depends-on**: task-001, task-006, task-007, task-008, task-009, task-010

## Description

Update the project Makefile to include vision module source files and platform-specific compilation flags.

## Execution Context

**Task Number**: 16 of 16
**Phase**: Configuration
**Prerequisites**: All implementation tasks should be complete

## BDD Scenario

This is a configuration task - no specific BDD scenario, but must ensure all platform capture backends compile correctly.

## Files to Modify/Create

- Modify: `Makefile`

## Steps

### Step 1: Add vision module variables
- Define VISION_SOURCES with all vision module files
- Define platform-specific sources based on detected OS

### Step 2: Add platform-specific flags
- Linux: Add -DHAVE_X11, -DHAVE_WAYLAND flags
- Linux X11: Add -lX11 -lXext to LDFLAGS
- Linux Wayland: Add -lwayland-client to LDFLAGS
- Windows: Add -lgdi32 to LDFLAGS
- macOS: Add -framework CoreGraphics -framework ApplicationServices

### Step 3: Add build targets
- Add vision.o to OBJECTS
- Add clean target for vision object files

### Step 4: Add detection logic
- Auto-detect platform from OS
- Enable appropriate capture backend

## Makefile Additions

```makefile
# Vision module sources
VISION_SOURCES = src/vision/vision.cpp \
                 src/vision/screen_capture.cpp \
                 src/vision/image_utils.cpp \
                 src/vision/tui.cpp \
                 src/vision/analysis.cpp

# Platform detection and sources
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    VISION_SOURCES += src/vision/platform/capture_x11.cpp
    # Check for Wayland/Hyprland at runtime
    VISION_CFLAGS += -DHAVE_X11
    VISION_LIBS += -lX11 -lXext
endif

ifeq ($(UNAME_S),Darwin)
    VISION_SOURCES += src/vision/platform/capture_mac.mm
    VISION_LIBS += -framework CoreGraphics -framework ApplicationServices
endif

# Add to OBJECTS
OBJECTS += $(VISION_SOURCES:.cpp=.o) $(VISION_SOURCES:.mm=.o)

# Add to LDFLAGS
LDFLAGS += $(VISION_LIBS)

# Add to CFLAGS
CFLAGS += $(VISION_CFLAGS)
```

## Verification Commands

```bash
# Clean and rebuild
make clean && make

# Verify vision module compiled
ls -la *.o | grep vision

# Test on different platform (if cross-compiling)
make CXX=x86_64-w64-mingw32-g++ WIN32=1
```

## Success Criteria

- Makefile compiles all vision module sources
- Platform-specific flags are applied correctly
- All platform backends can be enabled/disabled
- Clean target removes vision object files