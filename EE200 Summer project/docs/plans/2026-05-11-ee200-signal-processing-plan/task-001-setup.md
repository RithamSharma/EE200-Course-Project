# Task 001: Setup Project Structure

## Context

Create the project structure and verify all required dependencies are available in the Jupyter notebook environment.

## BDD Scenario

N/A - Setup task

## Goal

Prepare the Jupyter notebook environment with all required libraries and project header.

## Detailed Steps

1. **Create notebook structure**:
   - Create new Jupyter notebook `ee200_signal_processing.ipynb`
   - Set kernel to Python 3
   - Add markdown header: "EE200 Summer 2025 - Signal Processing Project"
   - Add description listing all topics covered

2. **Add import cell** with code:
   ```python
   import numpy as np
   import matplotlib.pyplot as plt
   from PIL import Image
   import librosa
   import librosa.display
   from scipy.fft import fft2, ifft2, fftshift
   import os
   ```

3. **Configure plotting**:
   ```python
   %matplotlib inline
   plt.style.use('default')
   ```

4. **Set up paths**:
   ```python
   base_path = os.getcwd()
   print(f"Working directory: {base_path}")
   ```

## Expected Output

- Notebook with Python 3 kernel
- All imports successful (no ModuleNotFoundError)
- Print statement showing working directory

## Verification

Run: `python -c "import numpy, matplotlib, PIL, librosa, scipy; print('All imports OK')"`

Success criteria:
- All 6 modules import without errors
- %matplotlib inline magic command works
- os.getcwd() returns valid path

## Files

- `ee200_signal_processing.ipynb` - Main notebook file (new)

## Notes

- Dependencies: numpy, matplotlib, pillow, librosa, scipy
- Assets are pre-existing: cat_gray.jpg, dog_gray.jpg, song_with_2piccolo.wav
- Reference: Basic_image_audio_tut.ipynb shows expected import style
