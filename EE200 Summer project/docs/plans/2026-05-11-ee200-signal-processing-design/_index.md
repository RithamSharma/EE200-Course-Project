---
name: EE200 Signal Processing Project Design
type: project
---

# EE200 Summer 2025 Signal Processing Project

## Context

This is an undergraduate signal processing course project covering fundamental concepts in both image and audio signal processing. The project uses Python with standard scientific computing libraries (NumPy, PIL, Matplotlib, Librosa, SciPy) to demonstrate core DSP concepts.

**Assets provided:**
- `cat_gray.jpg` - Grayscale cat image (400x400)
- `dog_gray.jpg` - Grayscale dog image (400x400)
- `song_with_2piccolo.wav` - Audio file with 2 piccolo melody

## Requirements

### Part A: Image Processing

1. **Basic Image Operations**
   - Load and display grayscale image
   - Convert color image to grayscale
   - Resize image (e.g., 200x200)
   - Crop image (coordinate-based)
   - Rotate image (45 degrees counter-clockwise)
   - Display all operations in subplot

2. **2D Discrete Fourier Transform (DFT)**
   - Compute 2D DFT of grayscale images
   - Display magnitude spectrum (shifted to center)
   - Display phase spectrum
   - Understand frequency domain representation

3. **Frequency Domain Filtering**
   - Design ideal low-pass filter (LPF)
   - Design ideal high-pass filter (HPF)
   - Apply filters in frequency domain
   - Compare original vs filtered images
   - Analyze blur vs edge enhancement effects

### Part B: Audio Signal Processing

1. **Basic Audio Loading**
   - Load audio file with librosa
   - Extract sampling rate and duration
   - Normalize audio signal

2. **Waveform Visualization**
   - Plot amplitude vs time
   - Display normalized waveform

3. **Time-Frequency Analysis**
   - Compute STFT (Short-Time Fourier Transform)
   - Display spectrogram
   - Convert to dB scale
   - Interpret frequency content over time

4. **Spectral Analysis**
   - Compute power spectral density
   - Identify dominant frequencies
   - Analyze harmonic content (for the piccolo melody)

## Complexity Assessment: **Medium**

Cross-module project with two distinct domains (image + audio), each with clear patterns to follow from the tutorial notebooks. No sub-agents needed - direct implementation.

## Design Documents

- [BDD Specifications](./bdd-specs.md) - Behavior scenarios and testing strategy
- [Architecture](./architecture.md) - System architecture and component details
- [Best Practices](./best-practices.md) - Security, performance, and code quality guidelines
