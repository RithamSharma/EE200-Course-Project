# Architecture - EE200 Signal Processing Project

## Project Structure

```
EE200 Summer project/
├── docs/plans/YYYY-MM-DD-ee200-signal-processing-design/
│   ├── _index.md              # This file
│   ├── bdd-specs.md           # BDD scenarios
│   └── architecture.md        # This file
├── EE200_practical_summer_2025.pdf  # Assignment specification
├── Basic_image_audio_tut.ipynb        # Reference tutorial
├── cat_gray.jpg               # Grayscale cat image
├── dog_gray.jpg               # Grayscale dog image
├── song_with_2piccolo.wav     # Audio file
└── ee200_signal_processing.ipynb  # Main project notebook
```

## Component Design

### Main Notebook Structure: `ee200_signal_processing.ipynb`

**Cell 1: Setup and Imports**
```python
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image
import librosa
import librosa.display
from scipy.fft import fft2, ifft2, fftshift
```

**Cell 2: Image Loading and Basic Operations**
- Load both cat and dog images
- Demonstrate resize, crop, rotate
- Display in subplot grid

**Cell 3: 2D DFT Implementation**
- Compute 2D FFT of images
- Shift zero frequency to center
- Display magnitude and phase spectra

**Cell 4: Frequency Domain Filtering**
- Create ideal LPF and HPF masks
- Apply filters in frequency domain
- Inverse FFT and display results
- Compare blur vs edge enhancement

**Cell 5: Audio Loading and Waveform**
- Load audio with librosa
- Display sampling rate and duration
- Plot normalized waveform

**Cell 6: Audio Spectrogram Analysis**
- Compute STFT
- Convert to dB spectrogram
- Display with proper colormap and colorbar

## Key Algorithms

### 2D DFT
```
F(u,v) = Σ Σ f(x,y) * e^(-j2π(ux/M + vy/N))
```

### Ideal Low-Pass Filter
```
H(u,v) = 1 if sqrt((u-M/2)² + (v-N/2)²) <= D₀
       = 0 otherwise
```

### Ideal High-Pass Filter
```
H(u,v) = 0 if sqrt((u-M/2)² + (v-N/2)²) <= D₀
       = 1 otherwise
```

### STFT for Audio
```
D(k,l) = Σₙ x(n) * w(n-kR) * e^(-j2πln/N)
```

## Dependencies

- Python 3.x
- numpy
- matplotlib
- PIL (Pillow)
- librosa
- scipy
- IPython/Jupyter
