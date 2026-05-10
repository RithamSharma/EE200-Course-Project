#include <vision/screen_capture.h>

namespace novashell::vision {

class Win32Capture : public ScreenCapture {
public:
  Win32Capture();
  ~Win32Capture() override;

  ImageResult capture() override;
  ImageResult capture_monitor(int index);
  bool is_supported() const override;

private:
  ImageResult capture_via_gdi();
};

} // namespace novashell::vision