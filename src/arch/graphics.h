#pragma once

#include <array>
#include <stdexcept>

namespace arch {
  namespace graphics {
    constexpr std::size_t screen_width{64};   // Number of pixels wide
    constexpr std::size_t screen_height{32};  // Number of pixels high

    class PixelCoordinateOutOfBounds : public std::exception {
    public:
      virtual const char* what() const noexcept {
        return "Pixel coordinate specified is out of bounds.\n";
      }
    };
  }  // namespace graphics

  class Graphics {
  public:
    Graphics();

    void set_pixel(std::size_t x_pos, std::size_t y_pos, bool pixel);

    bool get_pixel(std::size_t x_pos, std::size_t y_pos) const;

    void clear_screen() noexcept;

    bool draw_pixel(std::size_t x_pos, std::size_t y_pos, bool value);

  private:
    std::array<std::array<bool, graphics::screen_width>, graphics::screen_height>
        m_display_pixels{};  // row major order i.e. the inner array is the row
  };
}  // namespace arch
