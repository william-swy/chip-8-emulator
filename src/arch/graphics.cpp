#include "graphics.h"

arch::Graphics::Graphics() { clear_screen(); }

void arch::Graphics::set_pixel(std::size_t x_pos, std::size_t y_pos, bool pixel) {
  try {
    m_display_pixels.at(y_pos).at(x_pos) = pixel;
  } catch (const std::out_of_range&) {
    throw graphics::PixelCoordinateOutOfBounds();
  }
}

bool arch::Graphics::get_pixel(std::size_t x_pos, std::size_t y_pos) const {
  try {
    return m_display_pixels.at(y_pos).at(x_pos);
  } catch (const std::out_of_range&) {
    throw graphics::PixelCoordinateOutOfBounds();
  }
}

void arch::Graphics::clear_screen() noexcept {
  for (auto& row : m_display_pixels) {
    row.fill(false);
  }
}

bool arch::Graphics::draw_pixel(std::size_t x_pos, std::size_t y_pos, bool value) {
  const auto curr_pixel = get_pixel(x_pos, y_pos);
  const auto new_pixel = (curr_pixel != value);
  set_pixel(x_pos, y_pos, new_pixel);

  return curr_pixel && (!new_pixel);
}
