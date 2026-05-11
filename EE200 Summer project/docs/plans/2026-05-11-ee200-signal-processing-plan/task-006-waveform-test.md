# Task 006: Waveform Visualization Test

## Context

Verify audio waveform plotting with librosa.display.waveshow, ensuring correct time (x-axis) and amplitude (y-axis) representation.

## BDD Scenario

```gherkin
Scenario: Plot waveform
  Given audio signal and sampling rate
  When I plot using librosa.display.waveshow
  Then x-axis should show time in seconds
  And y-axis should show amplitude
```

## Goal

Test waveform visualization with proper normalization, axes labels, and grid.

## Detailed Steps

### Step 6.1: Normalize Audio

```python
# Normalize audio to [-1, 1] range
y_normalized = y / np.max(np.abs(y))

print(f"Normalized range: [{y_normalized.min():.4f}, {y_normalized.max():.4f}]")
```

### Step 6.2: Plot Waveform

```python
# Create figure
plt.figure(figsize=(12, 4))

# Plot waveform using librosa
librosa.display.waveshow(y_normalized, sr=sr)

# Add labels and title
plt.title("Audio Waveform - Song with 2 Piccolo")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")

# Add grid
plt.grid(True, alpha=0.3)

# Adjust layout
plt.tight_layout()
plt.show()
```

## Expected Output

- Console: Normalized range printed
- Figure: Waveform plot with time on x-axis (seconds), amplitude on y-axis (-1 to 1)

## Verification

1. **Normalization**:
   - `y_normalized.min() >= -1.0` ✓
   - `y_normalized.max() <= 1.0` ✓

2. **Waveform plot**:
   - x-axis shows time in seconds ✓
   - y-axis shows amplitude values ✓
   - Grid visible with transparency ✓
   - Title displayed correctly ✓

## Files

- `ee200_signal_processing.ipynb` - Cell 10

## depends-on

["005"]
