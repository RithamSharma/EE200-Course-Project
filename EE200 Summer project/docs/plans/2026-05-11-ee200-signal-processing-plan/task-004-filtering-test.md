# Task 004: Frequency Domain Filtering Test

## Context

Verify ideal low-pass filter (LPF) and high-pass filter (HPF) design, application in frequency domain, and visualization of blur vs edge enhancement effects.

## BDD Scenario

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

## Goal

Test ideal filter design and frequency domain application showing blur (LPF) and edge enhancement (HPF).

## Detailed Steps

### Step 4.1: Create Filter Function

```python
def create_ideal_filter(shape, cutoff, filter_type='low'):
    """
    Create an ideal filter in frequency domain.
    
    Parameters:
    - shape: tuple (M, N) image dimensions
    - cutoff: cutoff frequency D0
    - filter_type: 'low' for LPF, 'high' for HPF
    
    Returns:
    - filter mask of same shape
    """
    M, N = shape
    # Create coordinate grids centered at zero
    u = np.arange(-M//2, M//2)
    v = np.arange(-N//2, N//2)
    V, U = np.meshgrid(v, u)
    
    # Compute distance from center (frequency radius)
    D = np.sqrt(U**2 + V**2)
    
    if filter_type == 'low':
        # Ideal Low-Pass Filter: 1 inside radius, 0 outside
        H = (D <= cutoff).astype(float)
    else:  # high-pass
        # Ideal High-Pass Filter: 0 inside radius, 1 outside
        H = (D > cutoff).astype(float)
    
    return H
```

### Step 4.2: Design Filters with Clear Effects

The key to visible image transformation is choosing the right cutoff frequency:
- **Cutoff too high** (e.g., >50%): Minimal effect - almost identical to original
- **Cutoff too low** (e.g., <5%): Extreme blur or only edges visible
- **Optimal range**: 10-20% of image dimension for clear, educational effects

```python
# Get image dimensions
M, N = cat_array.shape
cutoff = min(M, N) // 8  # Cutoff at 12.5% of image dimension (~50 pixels for 400x400)

# Also create additional cutoff values for comparison
cutoff_aggressive = min(M, N) // 16  # 6.25% - stronger blur
cutoff_conservative = min(M, N) // 4   # 25% - subtle blur

print(f"Image dimensions: {M} x {N}")
print(f"Conservative cutoff (25%): {cutoff_conservative} pixels - subtle blur")
print(f"Default cutoff (12.5%): {cutoff} pixels - moderate effect")
print(f"Aggressive cutoff (6.25%): {cutoff_aggressive} pixels - strong blur")
```

### Step 4.3: Visualize Multiple Cutoff Filters

Display a row of filter masks at different cutoffs to show how the radius affects the result.

### Step 4.4: Apply Filters with Multiple Cutoff Comparison

Apply filters at all three cutoff levels to show the range of effects.

```python
# Apply LPF at different cutoffs
lpf_conservative = create_ideal_filter((M, N), cutoff_conservative, 'low')
lpf_default = create_ideal_filter((M, N), cutoff, 'low')
lpf_aggressive = create_ideal_filter((M, N), cutoff_aggressive, 'low')

cat_lpf_conservative = np.real(ifft2(fftshift(cat_fft_shift * lpf_conservative)))
cat_lpf_default = np.real(ifft2(fftshift(cat_fft_shift * lpf_default)))
cat_lpf_aggressive = np.real(ifft2(fftshift(cat_fft_shift * lpf_aggressive)))

# Apply HPF at default cutoff (HPF at different cutoffs looks similar - just edges)
hpf_default = create_ideal_filter((M, N), cutoff, 'high')
cat_hpf = np.real(ifft2(fftshift(cat_fft_shift * hpf_default)))
```

### Step 4.5: Apply to Dog Image

Repeat step 4.4 for dog image.

### Step 4.6: Display Results with Clear Comparison

Create 3x3 subplot showing multiple cutoff effects:

Row 1: Cat (Original, LPF Conservative, LPF Default)
Row 2: Cat (LPF Aggressive, HPF, side-by-side original-filtered)
Row 3: Dog (Original, LPF Default, HPF)

Add titles clearly indicating cutoff values. Use consistent figure size for fair comparison.

## Expected Output

- Console: Image dimensions, cutoff frequency printed
- Figure 1: Filter masks (white circle on black for LPF, black circle on white for HPF)
- Figure 2: 2x3 grid with original and filtered images

## Critical: Ensuring Visible Transformation Effects

**The most common issue in frequency domain filtering is choosing wrong cutoff values.**

To guarantee visible effects, we use three cutoff strategies:
1. **Conservative (25%)**: Subtle blur - demonstrates LPF without being distracting
2. **Default (12.5%)**: Clear educational effect - obvious but not extreme
3. **Aggressive (6.25%)**: Strong blur - shows dramatic low-pass effect

**LPF verification** (blur effect):
- After applying LPF, compare edge regions between original and filtered
- Blur should be most visible in fine-detail areas (fur texture, edges)
- The HPF result should show strong edges with dark (near-black) smooth regions

**HPF verification** (edge enhancement):
- HPF result should look like a line drawing of the original
- Smooth areas (single-color regions) should be dark/black
- Edges between different areas should be bright/highlighted

## Verification

1. **LPF mask verification**:
   - `np.all(lpf[D <= cutoff] == 1)` ✓
   - `np.all(lpf[D > cutoff] == 0)` ✓

2. **HPF mask verification**:
   - `np.all(hpf[D <= cutoff] == 0)` ✓
   - `np.all(hpf[D > cutoff] == 1)` ✓

3. **LPF effect - MUST BE VISIBLE**:
   - Image appears smoothed/blurred compared to original ✓
   - Fine details (fur texture) reduced ✓
   - Cannot just be a subtle difference - should be clearly noticeable

4. **HPF effect - MUST BE VISIBLE**:
   - Image shows primarily edges/lines ✓
   - Smooth areas become dark/near-black (not just slightly darker) ✓
   - Looks like a sketch/outline of the original ✓

5. **Side-by-side comparison**:
   - Original and filtered side-by-side with same figure size
   - Difference should be immediately obvious to observer

## Files

- `ee200_signal_processing.ipynb` - Cell 6 (filters), Cell 7 (apply), Cell 8 (display)

## depends-on

["003"]
