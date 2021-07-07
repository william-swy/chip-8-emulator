#include <iostream>
#include <string>

#include "chip8.h"
#include "display/display.h"
#include "display/keys.h"

constexpr unsigned int SCALING_FACTOR = 20;
constexpr unsigned int WINDOW_WIDTH = arch::graphics::screen_width * SCALING_FACTOR;
constexpr unsigned int WINDOW_HEIGHT = arch::graphics::screen_height * SCALING_FACTOR;

KeyPressed keys;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::string current_exec_name = argv[0];
    std::cout << "Usage: " << current_exec_name << " < path to rom to run > " << std::endl;
    return 1;
  }

  std::string rom_path = argv[1];

  display::Display display(WINDOW_WIDTH, WINDOW_HEIGHT);
  Chip8 emulator(rom_path);

  while (1) {
    if (display.stop_rendering()) {
      break;
    }

    emulator.emulate_cycle();

    if (emulator.should_draw()) {
      for (auto x = 0; x < arch::graphics::screen_width; x++) {
        for (auto y = 0; y < arch::graphics::screen_height; y++) {
          const auto pix = emulator.get_pixel(x, y);
          if (pix) {
            display.draw_scaled_pixel(255, 255, 255, x, y, SCALING_FACTOR);
          } else {
            display.draw_scaled_pixel(0, 0, 0, x, y, SCALING_FACTOR);
          }
        }
      }
      display.render_display();
    }

    keys = display.handle_keyboard();

    emulator.set_keys(keys);
  }
}