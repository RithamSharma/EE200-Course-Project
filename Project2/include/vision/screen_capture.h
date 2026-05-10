#pragma once

#include "types.h"

namespace novashell::vision {

class ScreenCapture {
public:
  virtual ~ScreenCapture() = default;
  virtual ImageResult capture() = 0;
  virtual bool is_supported() const = 0;
  static std::unique_ptr<ScreenCapture> create_platform_capture();
};

} // namespace novashell::vision