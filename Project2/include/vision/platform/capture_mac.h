#include <vision/screen_capture.h>

namespace novashell::vision {

class MacCapture : public ScreenCapture {
public:
  MacCapture();
  ~MacCapture() override;

  ImageResult capture() override;
  ImageResult capture_display(void* display_id);
  bool is_supported() const override;

private:
  ImageResult cgimage_to_imagedata(void* img);
};

} // namespace novashell::vision