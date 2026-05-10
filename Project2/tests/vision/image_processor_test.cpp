#include <vision/image_utils.h>
#include <cassert>
#include <cstring>

using namespace novashell::vision;

void test_format_detection() {
  uint8_t png_magic[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
  auto result = ImageProcessor::detect_format(png_magic, sizeof(png_magic));
  assert(result.ok());
  assert(result.value() == ImageFormat::PNG);
  printf("PASS: test_format_detection\n");
}

void test_jpeg_format() {
  uint8_t jpeg_magic[] = {0xFF, 0xD8, 0xFF, 0xE0};
  auto result = ImageProcessor::detect_format(jpeg_magic, sizeof(jpeg_magic));
  assert(result.ok());
  assert(result.value() == ImageFormat::JPEG);
  printf("PASS: test_jpeg_format\n");
}

void test_invalid_format() {
  uint8_t invalid[] = {0x00, 0x01, 0x02, 0x03};
  auto result = ImageProcessor::detect_format(invalid, sizeof(invalid));
  assert(!result.ok());
  printf("PASS: test_invalid_format\n");
}

void test_image_data() {
  ImageData img(100, 100, 4);
  assert(img.width == 100);
  assert(img.height == 100);
  assert(img.channels == 4);
  assert(img.data.size() == 40000);
  printf("PASS: test_image_data\n");
}

void test_result_type() {
  Result<int> ok_result(42);
  assert(ok_result.ok());
  assert(ok_result.value() == 42);

  Result<int> err_result(VisionError::InvalidImage);
  assert(!err_result.ok());
  assert(err_result.error() == VisionError::InvalidImage);
  printf("PASS: test_result_type\n");
}

int main() {
  test_format_detection();
  test_jpeg_format();
  test_invalid_format();
  test_image_data();
  test_result_type();
  printf("\nAll tests passed!\n");
  return 0;
}