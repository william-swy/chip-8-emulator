#include "graphics.h"

arch::Graphics::Graphics() { clear_screen(); }

void arch::Graphics::set_pixel(size_t x, size_t y, bool pixel) {
  if (x >= graphics::screen_width || y >= graphics::screen_height) {
    throw graphics::PixelCoordinateOutOfBounds();
  } else {
    const auto idx = y * graphics::screen_width + x;
    display_pixels[idx] = pixel;
  }
}

bool arch::Graphics::get_pixel(size_t x, size_t y) const {
  if (x >= graphics::screen_width || y >= graphics::screen_height) {
    throw graphics::PixelCoordinateOutOfBounds();
  } else {
    const auto idx = y * graphics::screen_width + x;
    return display_pixels[idx];
  }
}

void arch::Graphics::clear_screen() noexcept {
  for (auto& pixel : display_pixels) {
    pixel = false;
  }
}
