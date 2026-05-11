# Task 002: Basic Image Operations Test

## Context

Verify that basic image operations (load, resize, crop, rotate) produce correct outputs for both cat and dog grayscale images.

## BDD Scenario

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

## Goal

Test all basic image operations and display results in organized subplot grid.

## Detailed Steps

### Step 2.1: Load Images

```python
# Load grayscale images
cat_img = Image.open('cat_gray.jpg')
dog_img = Image.open('dog_gray.jpg')

# Convert to numpy arrays for processing
cat_array = np.array(cat_img)
dog_array = np.array(dog_img)

print(f"Cat image shape: {cat_array.shape}")
print(f"Dog image shape: {dog_array.shape}")
```

### Step 2.2: Display Original Images

Create 1x2 subplot showing both original images with gray colormap.

### Step 2.3: Resize Operations

```python
# Resize both images to 200x200
cat_resized = cat_img.resize((200, 200))
dog_resized = dog_img.resize((200, 200))
```

### Step 2.4: Crop Operations

```python
# Crop: (left, upper, right, lower) coordinates
cat_cropped = cat_img.crop((50, 50, 200, 200))
dog_cropped = dog_img.crop((50, 50, 200, 200))
```

### Step 2.5: Rotate Operations

```python
# Rotate by 45 degrees counter-clockwise
cat_rotated = cat_img.rotate(45)
dog_rotated = dog_img.rotate(45)
```

### Step 2.6: Display All Operations

Create 1x4 subplot for cat image: Original, Resized, Cropped, Rotated.
Repeat for dog image below.

## Expected Output

- Console: Image shapes printed (e.g., 400x400 or similar)
- Matplotlib figure: 1x4 grid showing all operations for cat
- Second figure: Same for dog

## Verification

1. **Load test**:
   - `Image.open()` returns valid PIL Image
   - `np.array()` converts to numpy array

2. **Resize test**:
   - `cat_resized.size == (200, 200)` ✓
   - `dog_resized.size == (200, 200)` ✓

3. **Crop test**:
   - `cat_cropped.size == (150, 150)` ✓
   - Coordinates (50, 50, 200, 200) produce 150x150 crop

4. **Rotate test**:
   - `cat_rotated` is PIL Image
   - Rotation appears at 45° angle

## Files

- `ee200_signal_processing.ipynb` - Cell 2 (load), Cell 3 (operations)

## depends-on

["001"]
