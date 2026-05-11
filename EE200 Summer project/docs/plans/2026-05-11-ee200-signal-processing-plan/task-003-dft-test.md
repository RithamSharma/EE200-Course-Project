# Task 003: 2D DFT Test

## Context

Verify 2D Discrete Fourier Transform computation, magnitude spectrum display, and phase spectrum visualization.

## BDD Scenario

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

## Goal

Test 2D DFT computation, display centered magnitude spectrum with log scale, and show phase spectrum.

## Detailed Steps

### Step 3.1: Compute 2D FFT

```python
# Compute 2D FFT for cat and dog images
cat_fft = fft2(cat_array)
dog_fft = fft2(dog_array)

# Shift zero frequency to center
cat_fft_shift = fftshift(cat_fft)
dog_fft_shift = fftshift(dog_fft)
```

### Step 3.2: Compute Magnitude Spectrum

```python
# Compute magnitude spectrum (log scale for better visualization)
cat_magnitude = np.log(1 + np.abs(cat_fft_shift))
dog_magnitude = np.log(1 + np.abs(dog_fft_shift))
```

### Step 3.3: Compute Phase Spectrum

```python
# Compute phase spectrum
cat_phase = np.angle(cat_fft_shift)
dog_phase = np.angle(dog_fft_shift)
```

### Step 3.4: Print Statistics

```python
print(f"FFT shape: {cat_fft.shape}")
print(f"Max magnitude (log): {cat_magnitude.max():.2f}")
print(f"Phase range: [{cat_phase.min():.2f}, {cat_phase.max():.2f}] radians")
```

### Step 3.5: Display Magnitude Spectra

Create 1x2 subplot with centered magnitude spectra.
Use `cmap='gray'` for display.
Add proper titles: "Cat - Magnitude Spectrum (Log)", "Dog - Magnitude Spectrum (Log)"

### Step 3.6: Display Phase Spectra

Create 1x2 subplot with phase spectra.
Use `cmap='twilight'` for better phase visualization.
Add colorbar showing phase values.

## Expected Output

- Console: FFT shape, max magnitude, phase range printed
- Figure 1: Magnitude spectra (log scale, centered)
- Figure 2: Phase spectra with colorbar

## Verification

1. **FFT computation**:
   - `cat_fft.dtype` is complex128 ✓
   - `cat_fft.shape == cat_array.shape` ✓

2. **Magnitude spectrum**:
   - Shows symmetric pattern around center ✓
   - Log scale displays low and high frequencies ✓
   - Bright center (DC component) visible ✓

3. **Phase spectrum**:
   - `cat_phase.min() >= -np.pi` ✓
   - `cat_phase.max() <= np.pi` ✓
   - Smooth variations across image ✓

## Files

- `ee200_signal_processing.ipynb` - Cell 4 (magnitude), Cell 5 (phase)

## depends-on

["002"]
