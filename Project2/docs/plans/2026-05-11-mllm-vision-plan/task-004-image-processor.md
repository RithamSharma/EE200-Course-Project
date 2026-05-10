# Task 004: Implement ImageProcessor utilities

**depends-on**: task-002

## Description

Implement image processing utilities including base64 encoding, image resizing, format conversion, and size optimization.

## Execution Context

**Task Number**: 4 of 16
**Phase**: Foundation
**Prerequisites**: Task 002 (types) must be complete

## BDD Scenario

```gherkin
Scenario: Base64 Encoding
  Given PNG image data
  When processing for API transmission
  Then encode to base64 string
  And ensure no data corruption during encoding
  And include proper MIME type in request
```

```gherkin
Scenario: Large Image Rejection
  Given the user provides an image file larger than 8MB
  When the image is being processed
  Then calculate estimated base64 size (file × 1.37)
  And if exceeds 5MB, resize to maximum 1920x1080 before encoding
  And show progress indicator during resize
  And proceed to analysis with optimized image
```

**Spec Source**: `../2026-05-11-mllm-vision-module/bdd-specs.md` - Image Processing Pipeline section

## Files to Modify/Create

- Create: `include/vision/image_utils.h`
- Create: `src/vision/image_utils.cpp`

## Steps

### Step 1: Define ImageProcessor class
- Create class with static methods:
  - `to_base64(const ImageData&)` → `std::string`
  - `from_base64(const std::string&, ImageFormat)` → `Result<ImageData>`
  - `resize(const ImageData&, int max_width, int max_height)` → `Result<ImageData>`
  - `convert_to_png(const ImageData&)` → `Result<ImageData>`
  - `validate_format(const std::string& path)` → `Result<ImageFormat>`

### Step 2: Implement base64 encoding
- Use standard base64 encoding algorithm
- Return proper MIME type along with base64 string

### Step 3: Implement image validation
- Check magic bytes for PNG (89 50 4E 47), JPEG (FF D8), BMP (42 4D), WebP (52 49 46 46)
- Return VisionError::InvalidImage for unrecognized formats

### Step 4: Implement size optimization
- Check estimated base64 size before encoding
- Resize if exceeds 5MB threshold
- Preserve aspect ratio during resize

## Interface Signatures

```cpp
class ImageProcessor {
public:
  struct ProcessedImage {
    std::string base64_data;
    std::string mime_type;
    size_t original_size;
    size_t processed_size;
  };
  
  static Result<ProcessedImage> prepare_for_api(const ImageData& img);
  static Result<ImageData> load_from_file(const std::string& path);
  static Result<ImageData> resize(const ImageData& img, int max_w, int max_h);
  static Result<ImageData> convert_to_png(const ImageData& img);
  static Result<ImageFormat> detect_format(const uint8_t* data, size_t size);
};
```

## Verification Commands

```bash
# Compile image utils
g++ -std=c++20 -c src/vision/image_utils.cpp -I include

# Run unit tests (when tests are added)
./build/vision_tests --run image_processor
```

## Success Criteria

- Base64 encoding produces valid output
- Format validation detects supported formats
- Resize maintains aspect ratio
- Large images are automatically optimized