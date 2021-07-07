#pragma once
#ifndef CHIP8EMULATOR_DISPLAY_DISPLAY_H_
#  define CHIP8EMULATOR_DISPLAY_DISPLAY_H_

#  include <memory>

#  include "keys.h"

namespace display {
  class Display {
  public:
    Display(int screen_width, int screen_height);

    ~Display();

    // Should not be cloned so disable copy and assignment
    Display(const Display&) = delete;

    Display& operator=(const Display&) = delete;

    void draw_pixel(unsigned char red, unsigned char green, unsigned char blue, int x_pos,
                    int y_pos) const;

    void draw_scaled_pixel(unsigned char red, unsigned char green, unsigned char blue,
                           int original_x_pos, int original_y_pos,
                           unsigned int scaling_factor) const;

    void render_display() const;

    void delay(unsigned int milli_sec) const;

    bool stop_rendering() const;

    KeyPressed handle_keyboard() const;

  private:
    class sdl_pimpl;

    std::unique_ptr<sdl_pimpl> p_impl;
  };
}  // namespace display
#endif  // !CHIP8EMULATOR_DISPLAY_DISPLAY_H_