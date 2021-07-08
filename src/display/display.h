#pragma once

#include <memory>

#include "input_events.h"

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

    enum input_events::Events handle_input() const;

  private:
    class sdl_pimpl;

    std::unique_ptr<sdl_pimpl> p_impl;
  };
}  // namespace display
