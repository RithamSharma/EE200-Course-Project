# BDD Specifications - EE200 Signal Processing Project

## Image Processing Features

### Feature: Basic Image Operations
```gherkin
Scenario: Load and display grayscale image
  Given a grayscale image file exists
  When I load the image using PIL
  Then I should have a valid Image object
  And the image should display correctly

Scenario: Convert image to grayscale
  Given a valid image object
  When I convert to grayscale using convert('L')
  Then the resulting image should have single channel
  And display in grayscale colormap

Scenario: Resize image to target dimensions
  Given a valid image object
  When I resize to (200, 200)
  Then the resulting image should be 200x200 pixels

Scenario: Crop image region
  Given a valid image object
  When I crop coordinates (50, 50, 200, 200)
  Then the resulting image should be 150x150 pixels
  And contain the specified region

Scenario: Rotate image by 45 degrees
  Given a valid image object
  When I rotate by 45 degrees
  Then the resulting image should display rotated content
```

### Feature: 2D Discrete Fourier Transform
```gherkin
Scenario: Compute 2D DFT of grayscale image
  Given a grayscale image as numpy array
  When I compute 2D FFT using np.fft.fft2
  Then I should get complex frequency domain representation
  And magnitude should be shifted to center

Scenario: Display magnitude spectrum
  Given 2D FFT result
  When I compute magnitude and shift
  Then magnitude spectrum should be centered
  And display logarithmic scale for visibility

Scenario: Display phase spectrum
  Given 2D FFT result
  When I compute phase angle
  Then phase should be in range [-π, π]
```

### Feature: Frequency Domain Filtering
```gherkin
Scenario: Design ideal low-pass filter
  Given image dimensions and cutoff frequency
  When I create ideal LPF mask
  Then pixels inside cutoff radius = 1
  And pixels outside cutoff radius = 0

Scenario: Apply low-pass filter
  Given image in frequency domain and LPF mask
  When I multiply and inverse transform
  Then resulting image should be smoother/blurred
  And low frequencies preserved

Scenario: Design ideal high-pass filter
  Given image dimensions and cutoff frequency
  When I create ideal HPF mask
  Then pixels inside cutoff radius = 0
  And pixels outside cutoff radius = 1

Scenario: Apply high-pass filter
  Given image in frequency domain and HPF mask
  When I multiply and inverse transform
  Then resulting image should show edges
  And low frequencies removed
```

## Audio Processing Features

### Feature: Basic Audio Loading
```gherkin
Scenario: Load audio file
  Given a valid WAV audio file
  When I load with librosa.load
  Then I should get audio signal array and sampling rate
  And sampling rate should be positive integer

Scenario: Extract audio metadata
  Given loaded audio signal
  When I compute duration
  Then duration should equal length/sampling_rate
```

### Feature: Waveform Visualization
```gherkin
Scenario: Plot waveform
  Given audio signal and sampling rate
  When I plot using librosa.display.waveshow
  Then x-axis should show time in seconds
  And y-axis should show amplitude
```

### Feature: Time-Frequency Analysis
```gherkin
Scenario: Compute STFT
  Given audio signal
  When I compute STFT with librosa.stft
  Then I should get complex STFT matrix
  And dimensions related to window and signal length

Scenario: Display spectrogram
  Given STFT result
  When I convert to dB scale
  Then spectrogram should show frequency vs time
  And colorbar should show dB scale

Scenario: Identify frequency content
  Given spectrogram
  When I examine dominant frequencies
  Then I should see content in expected piccolo range (500Hz-4kHz)
```

## Success Criteria

1. All images display correctly with proper titles and axes
2. DFT magnitude spectrum shows symmetric pattern
3. LPF creates visible blur effect
4. HPF enhances edges
5. Audio waveform plots correctly with time axis
6. Spectrogram shows clear time-frequency representation
7. Code runs without errors on provided assets
