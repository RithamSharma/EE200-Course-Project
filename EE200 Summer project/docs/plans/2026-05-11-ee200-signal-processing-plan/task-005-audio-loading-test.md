# Task 005: Audio Loading Test

## Context

Verify audio file loading with librosa, metadata extraction (sampling rate, duration), and signal properties display.

## BDD Scenario

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

## Goal

Test audio loading, verify sampling rate, compute and display duration.

## Detailed Steps

### Step 5.1: Load Audio File

```python
# Load the audio file
audio_path = 'song_with_2piccolo.wav'
y, sr = librosa.load(audio_path, sr=None)  # sr=None keeps original sampling rate
```

### Step 5.2: Display Metadata

```python
# Compute duration
duration = len(y) / sr

# Display audio metadata
print(f"Audio file: {audio_path}")
print(f"Sampling rate: {sr} Hz")
print(f"Duration: {duration:.2f} seconds")
print(f"Number of samples: {len(y)}")
print(f"Signal dtype: {y.dtype}")
print(f"Signal range: [{y.min():.4f}, {y.max():.4f}]")
```

## Expected Output

- Console: All metadata printed
- No visualization needed for this task

## Verification

1. **Audio loading**:
   - `y` is numpy array ✓
   - `sr` is positive integer (typically 22050 or 44100 Hz) ✓

2. **Duration calculation**:
   - `duration == len(y) / sr` ✓
   - Duration is reasonable (a few seconds to minutes)

3. **Signal properties**:
   - `y.dtype` is float32 or float64 ✓
   - `y.min()` and `y.max()` within [-1, 1] or similar audio range ✓

## Files

- `ee200_signal_processing.ipynb` - Cell 9

## depends-on

["001"]
