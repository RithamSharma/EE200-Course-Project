#include <vision/screen_capture.h>

namespace novashell::vision {

class WaylandCapture : public ScreenCapture {
public:
  WaylandCapture();
  ~WaylandCapture() override;

  ImageResult capture() override;
  bool is_supported() const override;

private:
  ImageResult capture_via_screencopy();
  ImageResult capture_via_gnome_screenshot();
};

} // namespace novashell::vision