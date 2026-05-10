# MLLM Vision Module Design

## Context

NovaShell (v3.0) currently supports text-only AI interactions via Google Gemini. The user wants to add vision capabilities for multimodal AI - analyzing screenshots, terminal visuals, and providing screen understanding, OCR, accessibility audit, and full analysis.

**User Requirements:**
- Interactive TUI picker for image selection
- Screen capture via custom per-platform implementation
- Support: Linux X11, Linux Wayland, Linux Hyprland, Windows (Win32/Win64), macOS
- Analysis modes: screen understanding, OCR, accessibility audit, full analysis
- Module-based architecture

## Requirements

1. **Core Feature**: New `see` command with interactive TUI for image capture/selection
2. **Screen Capture**: Platform-specific implementations for all major desktop OSes
3. **Image Processing**: Base64 encoding, format conversion, compression for API efficiency
4. **AI Integration**: Multi-mode analysis using Gemini's vision capabilities
5. **Architecture**: Modular design with separate components per concern

## Rationale

- **Interactive TUI** chosen over simple CLI for better UX - users can preview, annotate, select regions
- **Custom per-platform** chosen for best native experience vs portability trade-off
- **Module-based** for fast performance while keeping code organization clean

## Detailed Design

### Component Architecture

```
include/vision/
├── vision.h              # Main interface (SeeCommand class)
├── screen_capture.h     # Abstract base for capture
├── image_utils.h        # Base64, compression, format conversion
└── analysis.h            # Analysis mode definitions

src/vision/
├── vision.cpp           # SeeCommand implementation
├── screen_capture_x11.cpp
├── screen_capture_wayland.cpp
├── screen_capture_hyprland.cpp
├── screen_capture_win32.cpp
├── screen_capture_mac.mm
├── image_utils.cpp
└── analysis.cpp
```

### Key Classes

1. **SeeCommand**: Orchestrates capture → process → analyze flow
2. **ScreenCapture (abstract)**: Base class with `capture()` virtual method
3. **ImageProcessor**: Handles base64 encoding, PNG compression, size limits
4. **AnalysisMode**: Enum class for different analysis types

### API Integration

Extends existing `call_gemini_api()` with image support:
- Modifies JSON payload to include `inlineData` with base64 image
- Supports MIME types: image/png, image/jpeg, image/webp
- Preserves existing authentication flow

## Design Documents

- [BDD Specifications](./bdd-specs.md) - Behavior scenarios and testing strategy
- [Architecture](./architecture.md) - System architecture and component details
- [Best Practices](./best-practices.md) - Security, performance, and code quality guidelines