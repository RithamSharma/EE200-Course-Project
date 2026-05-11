# Task 007: Time-Frequency Analysis Test

## Context

Verify Short-Time Fourier Transform (STFT) computation, spectrogram display in dB scale, and dominant frequency identification for the piccolo melody.

## BDD Scenario

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

## Goal

Test STFT, dB spectrogram visualization, and dominant frequency analysis identifying piccolo harmonics.

## Detailed Steps

### Step 7.1: Compute STFT

```python
# Compute Short-Time Fourier Transform (STFT)
D = librosa.stft(y)

print(f"STFT shape: {D.shape}")
print(f"STFT dtype: {D.dtype}")
```

### Step 7.2: Convert to dB Scale

```python
# Convert to dB scale
S_db = librosa.amplitude_to_db(np.abs(D), ref=np.max)

print(f"dB range: [{S_db.min():.2f}, {S_db.max():.2f}] dB")
```

### Step 7.3: Plot Spectrogram

```python
# Create figure
plt.figure(figsize=(12, 5))

# Plot spectrogram
librosa.display.specshow(S_db, sr=sr, x_axis='time', y_axis='hz', cmap='magma')

# Add colorbar with dB format
plt.colorbar(format='%+2.0f dB')

# Add labels
plt.title("Spectrogram - Song with 2 Piccolo")
plt.xlabel("Time (s)")
plt.ylabel("Frequency (Hz)")

# Adjust layout
plt.tight_layout()
plt.show()
```

### Step 7.4: Compute Mean Spectrum

```python
# Compute mean spectrum across time
mean_spectrum = np.mean(np.abs(D), axis=1)
frequencies = librosa.fft_frequencies(sr=sr)
```

### Step 7.5: Plot Power Spectral Density

```python
# Plot power spectral density
plt.figure(figsize=(12, 4))
plt.semilogy(frequencies, mean_spectrum)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Mean Magnitude')
plt.title('Average Power Spectrum')
plt.grid(True, alpha=0.3)
plt.xlim([0, sr/2])  # Only show up to Nyquist frequency
plt.tight_layout()
plt.show()
```

### Step 7.6: Find Peak Frequencies

```python
from scipy.signal import find_peaks

# Find peaks in mean spectrum
peaks, properties = find_peaks(mean_spectrum, height=np.max(mean_spectrum)*0.1)
peak_frequencies = frequencies[peaks]
peak_magnitudes = mean_spectrum[peaks]

# Sort by magnitude and show top 10
sorted_idx = np.argsort(peak_magnitudes)[::-1][:10]
print("\nTop 10 Dominant Frequencies:")
print("-" * 30)
for i, idx in enumerate(sorted_idx):
    print(f"{i+1}. {peak_frequencies[idx]:.1f} Hz (magnitude: {peak_magnitudes[idx]:.4f})")
```

## Expected Output

- Console: STFT shape, dB range, top 10 frequencies
- Figure 1: Spectrogram with colorbar (magma colormap)
- Figure 2: Power spectral density plot

## Verification

1. **STFT computation**:
   - `D.dtype` is complex64 or complex128 ✓
   - `D.shape[0]` relates to FFT window size ✓
   - `D.shape[1]` relates to time frames ✓

2. **Spectrogram display**:
   - Frequency on y-axis (Hz) ✓
   - Time on x-axis (s) ✓
   - Colorbar shows dB scale ✓
   - magma colormap applied ✓

3. **Frequency identification**:
   - Peak frequencies printed ✓
   - Some peaks in 500Hz-4kHz range (piccolo range) ✓

## Files

- `ee200_signal_processing.ipynb` - Cell 11 (spectrogram), Cell 12 (spectral analysis)

## depends-on

["006"]
