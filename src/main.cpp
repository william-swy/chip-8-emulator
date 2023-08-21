#include <chrono>
#include <iostream>
#include <span>
#include <string>

#include "chip8.h"
#include "display/display.h"
#include "display/input_events.h"
#include "sound.h"

constexpr unsigned int SCALING_FACTOR{20};
constexpr unsigned int WINDOW_WIDTH{arch::graphics::screen_width
                                    * SCALING_FACTOR};  // Width of screen in px
constexpr unsigned int WINDOW_HEIGHT{arch::graphics::screen_height
                                     * SCALING_FACTOR};  // Height of screen in px

constexpr std::chrono::microseconds time_per_instruction{1000};

int main(int argc, char** argv) {
  const auto args = std::span(argv, std::size_t(argc));
  if (argc != 2) {
    std::string current_exec_name = args[0];
    std::cout << "Usage: " << current_exec_name << " < path to rom to run > " << std::endl;
    return 1;
  }

  std::string rom_path = args[1];

  display::Display display(WINDOW_WIDTH, WINDOW_HEIGHT);
  sdl_interface::Audio audio{};
  Chip8 emulator(rom_path);

  while (true) {
    const auto start_time{std::chrono::steady_clock::now()};

    const enum input_events::Events inputted_event = display.handle_input();

    if (inputted_event == input_events::Events::quit) {
      break;
    }

    emulator.handle_keys(inputted_event);

    emulator.emulate_cycle();

    if (emulator.should_buzz()) {
      audio.buzzer_on();
    } else {
      audio.buzzer_off();
    }

    if (emulator.should_draw()) {
      for (auto x = 0; x < arch::graphics::screen_width; x++) {
        for (auto y = 0; y < arch::graphics::screen_height; y++) {
          const auto pix = emulator.get_pixel(x, y);
          if (pix) {
            display.draw_scaled_pixel(255, 255, 255, x, y, SCALING_FACTOR);  // Color white
          } else {
            display.draw_scaled_pixel(0, 0, 0, x, y, SCALING_FACTOR);  // Colour black
          }
        }
      }
      display.render_display();
    }
    const auto end_time{std::chrono::steady_clock::now()};

    const auto elapsed_time{end_time - start_time};
    if (elapsed_time < time_per_instruction) {
      std::this_thread::sleep_for(time_per_instruction - elapsed_time);
    }
  }
}