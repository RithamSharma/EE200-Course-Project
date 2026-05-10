#include <vision/types.h>
#include <cassert>
#include <cstdio>
#include <cstring>

using namespace novashell::vision;

void test_error_messages() {
  assert(strcmp(error_message(VisionError::NoAuth), "No authentication configured. Use 'login' or 'set-key'.") == 0);
  assert(strcmp(error_message(VisionError::CaptureFailed), "Screen capture failed. Check permissions.") == 0);
  assert(strcmp(error_message(VisionError::InvalidImage), "Invalid image file or unsupported format.") == 0);
  assert(strcmp(error_message(VisionError::ImageTooLarge), "Image exceeds maximum size limit.") == 0);
  assert(strcmp(error_message(VisionError::ApiError), "API error occurred.") == 0);
  assert(strcmp(error_message(VisionError::Timeout), "Request timed out.") == 0);
  assert(strcmp(error_message(VisionError::PlatformNotSupported), "Screen capture not supported on this platform.") == 0);
  printf("PASS: test_error_messages\n");
}

void test_image_format_enum() {
  assert(static_cast<int>(ImageFormat::PNG) == 0);
  assert(static_cast<int>(ImageFormat::JPEG) == 1);
  assert(static_cast<int>(ImageFormat::BMP) == 2);
  assert(static_cast<int>(ImageFormat::WebP) == 3);
  assert(static_cast<int>(ImageFormat::Unknown) == 4);
  printf("PASS: test_image_format_enum\n");
}

void test_vision_error_enum() {
  assert(static_cast<int>(VisionError::NoAuth) == 0);
  assert(static_cast<int>(VisionError::CaptureFailed) == 1);
  assert(static_cast<int>(VisionError::InvalidImage) == 2);
  assert(static_cast<int>(VisionError::ImageTooLarge) == 3);
  assert(static_cast<int>(VisionError::ApiError) == 4);
  assert(static_cast<int>(VisionError::Timeout) == 5);
  assert(static_cast<int>(VisionError::PlatformNotSupported) == 6);
  printf("PASS: test_vision_error_enum\n");
}

int main() {
  test_error_messages();
  test_image_format_enum();
  test_vision_error_enum();
  printf("\nAll error handling tests passed!\n");
  return 0;
}