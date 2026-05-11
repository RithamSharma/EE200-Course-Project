# EE200 Signal Processing Implementation Plan

## Context

This plan implements the EE200 Summer 2025 signal processing project based on the assignment requirements. The project covers image transforms (DFT, frequency filtering) and audio analysis (waveform, spectrogram).

**Current State**: Basic tutorial notebooks exist (`Basic_image_audio_tut.ipynb`) but lack the comprehensive DFT filtering and spectral analysis required by the assignment.

**Target State**: Complete Jupyter notebook demonstrating:
- Part A: Image operations, 2D DFT, frequency domain filtering (LPF/HPF)
- Part B: Audio loading, waveform visualization, time-frequency analysis

## Goal

Create a comprehensive signal processing notebook that satisfies all assignment requirements and produces correct visualizations for both image and audio processing tasks.

## Execution Plan

```yaml
tasks:
  - id: "001"
    subject: "Setup project structure"
    slug: "setup-project-structure"
    type: "setup"
    depends-on: []
  - id: "002"
    subject: "Basic Image Operations Test"
    slug: "basic-image-operations-test"
    type: "test"
    depends-on: ["001"]
  - id: "003"
    subject: "2D DFT Test"
    slug: "2d-dft-test"
    type: "test"
    depends-on: ["002"]
  - id: "004"
    subject: "Frequency Domain Filtering Test"
    slug: "frequency-domain-filtering-test"
    type: "test"
    depends-on: ["003"]
  - id: "005"
    subject: "Audio Loading Test"
    slug: "audio-loading-test"
    type: "test"
    depends-on: ["001"]
  - id: "006"
    subject: "Waveform Visualization Test"
    slug: "waveform-visualization-test"
    type: "test"
    depends-on: ["005"]
  - id: "007"
    subject: "Time-Frequency Analysis Test"
    slug: "time-frequency-analysis-test"
    type: "test"
    depends-on: ["006"]
  - id: "008"
    subject: "Summary and Documentation"
    slug: "summary-documentation"
    type: "documentation"
    depends-on: ["004", "007"]
```

## Task File References

- [Task 001: Setup Project Structure](./task-001-setup.md)
- [Task 002: Basic Image Operations Test](./task-002-image-operations-test.md)
- [Task 003: 2D DFT Test](./task-003-dft-test.md)
- [Task 004: Frequency Domain Filtering Test](./task-004-filtering-test.md)
- [Task 005: Audio Loading Test](./task-005-audio-loading-test.md)
- [Task 006: Waveform Visualization Test](./task-006-waveform-test.md)
- [Task 007: Time-Frequency Analysis Test](./task-007-spectrogram-test.md)
- [Task 008: Summary and Documentation](./task-008-summary.md)

## BDD Coverage

| Feature | Scenario | Tasks Covered |
|---------|----------|---------------|
| Basic Image Operations | Load grayscale image | 001, 002 |
| Basic Image Operations | Convert to grayscale | 002 |
| Basic Image Operations | Resize to 200x200 | 002 |
| Basic Image Operations | Crop region | 002 |
| Basic Image Operations | Rotate 45° | 002 |
| 2D DFT | Compute 2D FFT | 003 |
| 2D DFT | Display magnitude spectrum | 003 |
| 2D DFT | Display phase spectrum | 003 |
| Frequency Domain Filtering | Design ideal LPF | 004 |
| Frequency Domain Filtering | Apply LPF | 004 |
| Frequency Domain Filtering | Design ideal HPF | 004 |
| Frequency Domain Filtering | Apply HPF | 004 |
| Basic Audio Loading | Load audio file | 001, 005 |
| Basic Audio Loading | Extract metadata | 005 |
| Waveform Visualization | Plot waveform | 006 |
| Time-Frequency Analysis | Compute STFT | 007 |
| Time-Frequency Analysis | Display spectrogram | 007 |
| Time-Frequency Analysis | Identify frequencies | 007 |

All 19 BDD scenarios are covered by the 8 tasks.

## Dependency Chain

```
001 (setup)
  ├── 002 (image ops)
  │     └── 003 (dft)
  │           └── 004 (filtering)
  └── 005 (audio loading)
        └── 006 (waveform)
              └── 007 (spectrogram)
                    └── 008 (summary)
                          └── [004, 007]
```

## Files to Modify

- `ee200_signal_processing.ipynb` - Main project notebook (create/update)

## Critical: Ensuring Visible Transformation Effects

**The most important aspect of this project is that the filtering effects must be clearly visible.**

Common student mistake: choosing cutoff values that are too high, resulting in filtered images that look almost identical to the originals.

**Solution**: Use three cutoff levels (6.25%, 12.5%, 25%) to guarantee visible effects at each level.

## Constraint: Visual Verification Required

All filtered images must show obvious, immediate visual differences:
- **LPF**: Image must appear clearly blurred/smoothed - not just slightly softer
- **HPF**: Image must appear as edge outlines - smooth regions must be dark/black
