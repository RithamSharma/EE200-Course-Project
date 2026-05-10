#include <vision/screen_capture.h>

namespace novashell::vision {

class X11Capture : public ScreenCapture {
public:
  X11Capture();
  ~X11Capture() override;

  ImageResult capture() override;
  bool is_supported() const override;

private:
  // X11-specific members will be added in full implementation
  void* display_ = nullptr;
};

} // namespace novashell::vision