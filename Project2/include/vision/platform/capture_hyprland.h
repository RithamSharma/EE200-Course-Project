#include <vision/screen_capture.h>

namespace novashell::vision {

class HyprlandCapture : public ScreenCapture {
public:
  HyprlandCapture();
  ~HyprlandCapture() override;

  ImageResult capture() override;
  bool is_supported() const override;

private:
  ImageResult capture_via_hyprctl();
  ImageResult capture_via_grim();
  std::string get_hyprsocket_path();
};

} // namespace novashell::vision