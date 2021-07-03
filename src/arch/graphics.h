#pragma once
#ifndef CHIP8EMULATOR_ARCH_GRAPHICS_H_
#  define CHIP8EMULATOR_ARCH_GRAPHICS_H_

#  include <array>
#  include <stdexcept>

namespace arch {
  namespace graphics {
    constexpr size_t screen_width = 64;                            // Number of pixels wide
    constexpr size_t screen_height = 32;                           // Number of pixels hight
    constexpr size_t total_pixels = screen_height * screen_width;  // Total pixels

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

    void set_pixel(size_t x, size_t y, bool pixel);

    bool get_pixel(size_t x, size_t y) const;

    void clear_screen() noexcept;

    bool draw_pixel(size_t x, size_t y, bool value);

  private:
    std::array<bool, graphics::total_pixels> display_pixels;
  };
}  // namespace arch
#endif  // !CHIP8EMULATOR_ARCH_GRAPHICS_H_
