# Best Practices - EE200 Signal Processing Project

## Image Processing

### DFT Best Practices
1. **Always shift** the FFT result with `fftshift()` before displaying magnitude spectrum
2. **Use logarithmic scale** (log(1 + magnitude)) for better visualization of dynamic range
3. **Normalize images** before FFT for consistent results
4. **Match filter size** to image size (M x N)

### Filtering Best Practices
1. **Choose appropriate cutoff frequency** - too low = excessive blur, too high = minimal effect
2. **Typical cutoff**: 10-30% of image dimension for visible effects
3. **Use real() after inverse FFT** to remove small imaginary artifacts from floating point errors
4. **Take absolute value** after inverse transform for display

### Display Best Practices
1. Use `cmap='gray'` for grayscale images
2. Use `figsize` appropriate for content
3. Add clear titles and axis labels
4. Use `plt.tight_layout()` to prevent overlap

## Audio Processing

### Loading Best Practices
1. **Keep original sampling rate** (sr=None) unless specifically needed
2. **Normalize audio** (y / max(|y|)) for consistent amplitude across files
3. **Handle stereo** - convert to mono if needed with `.mean(axis=1)`

### Spectrogram Best Practices
1. **Use amplitude_to_db** with ref=np.max for consistent scaling
2. **Choose appropriate hop_length** - smaller = better time resolution, larger = better frequency resolution
3. **Use 'magma' or 'viridis' colormap** for scientific visualization

### General
1. Add cell comments explaining key operations
2. Display intermediate results for verification
3. Use consistent figure sizes within related cells
