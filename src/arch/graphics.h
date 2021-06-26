#pragma once
#ifndef CHIP8EMULATOR_ARCH_GRAPHICS_H_
#  define CHIP8EMULATOR_ARCH_GRAPHICS_H_

#  include <array>

namespace arch {
  class Graphics {
  public:
    std::array<unsigned char, total_pixels> display_pixels;

  private:
    constexpr size_t screen_width = 64;                            // Number of pixels wide
    constexpr size_t screen_height = 32;                           // Number of pixels hight
    constexpr size_t total_pixels = screen_height * screen_width;  // Total pixels
  };
}  // namespace arch
#endif  // !CHIP8EMULATOR_ARCH_GRAPHICS_H_
