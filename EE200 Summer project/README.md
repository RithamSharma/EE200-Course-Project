# EE200 Summer 2025 - Signal Processing Project

A comprehensive signal processing project covering image transforms, audio analysis, and frequency domain processing.

## Project Overview

This project demonstrates fundamental signal processing concepts using Python with NumPy, SciPy, PIL, Matplotlib, and Librosa.

### Part A: Image Processing
- **Basic Operations**: Load, resize (200x200), crop, rotate (45°)
- **2D DFT**: Discrete Fourier Transform with magnitude and phase spectra
- **Frequency Filtering**: Ideal Low-Pass and High-Pass filters at multiple cutoffs (6.25%, 12.5%, 25%)
- **Rotation Analysis**: 90° anti-clockwise rotation with FFT comparison
- **Frequency Mixer**: Creative image fusion combining structure from one image with details from another

### Part B: Audio Processing
- **Audio Loading**: Load audio with Librosa, extract sampling rate and duration
- **Audio Restoration**: FFT-based LPF/HPF filtering with saved output files
- **Waveform Visualization**: Normalized amplitude vs time plot
- **Spectrogram**: STFT with dB scale time-frequency analysis
- **Spectral Analysis**: Power spectral density and dominant frequency identification

## Files

| File | Description |
|------|-------------|
| `ee200_signal_processing.ipynb` | Main Jupyter notebook with all implementations |
| `EE200_Signal_Processing_Report.tex` | LaTeX report with mathematical expressions |
| `report_images/` | 13 high-quality PNG figures for the report |
| `Basic_image_audio_tut.ipynb` | Reference tutorial notebook |

## Assets
- `cat_gray.jpg` - Grayscale cat image (361x410)
- `dog_gray.jpg` - Grayscale dog image (361x410)
- `song_with_2piccolo.wav` - Audio file with 2 piccolo melody

## Generated Output Files
- `audio_original.wav` - Original audio
- `audio_LPF_restored.wav` - Low-pass filtered audio
- `audio_HPF_restored.wav` - High-pass filtered audio

## Key Mathematical Concepts

### 2D Discrete Fourier Transform
```
F(u,v) = Σₓ Σᵧ f(x,y) × e^(-j2π(ux/M + vy/N))
```

### Ideal Low-Pass Filter
```
H_LPF(u,v) = 1 if √(u² + v²) ≤ D₀, else 0
```

### Ideal High-Pass Filter
```
H_HPF(u,v) = 0 if √(u² + v²) ≤ D₀, else 1
```

### Frequency Mixer (Image Fusion)
```
F_mixed(u,v) = F_A(u,v) × H_LPF(u,v) + F_B(u,v) × H_HPF(u,v)
```

### STFT for Audio
```
D(k,l) = Σₙ x(n) × w(n-lR) × e^(-j2πkn/N)
```

## Installation

```bash
pip install numpy matplotlib pillow librosa scipy jupyter
```

## Usage

```bash
jupyter notebook ee200_signal_processing.ipynb
```

Run all cells in order (Cell 1 through Cell 16) to see:
1. Image loading and basic operations
2. 2D DFT magnitude and phase spectra
3. Frequency domain filtering with multiple cutoffs
4. Rotation property demonstration
5. Frequency Mixer for creative image fusion
6. Audio loading and restoration
7. Waveform and spectrogram visualization
8. Spectral analysis with dominant frequency identification

## Compilation

To compile the LaTeX report:

```bash
pdflatex EE200_Signal_Processing_Report.tex
```

Requires: texlive-latex-base, amsmath, graphicx packages.

## Results

### Image Processing
- **LPF Effect**: Removes high frequencies → Blurred/smoothed image
- **HPF Effect**: Removes low frequencies → Edge enhancement
- **Frequency Mixer**: Creates hybrid images with structure from one image and texture from another

### Audio Processing
- **LPF Filtered**: Removes high-frequency noise/hiss
- **HPF Filtered**: Removes low-frequency rumble
- **Dominant Frequencies**: Identified in piccolo range (500Hz - 4kHz)

## Course Information

- **Course**: EE200 - Signal Processing
- **Semester**: Summer 2025
- **Department**: Electrical Engineering

## License

This project is for educational purposes.
