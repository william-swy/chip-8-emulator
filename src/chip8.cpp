#include "chip8.h"

#include <fstream>
#include <string>
#include <vector>

Chip8::Chip8(std::string& file_name) {
  cpu = arch::CPU{};
  memory = arch::Memory{};
  keypad = arch::Keypad{};
  graphics = arch::Graphics{};

  // Load font set into memory
  for (auto i = 0; i < chip8_fontset.size(); i++) {
    memory.set_value(static_cast<unsigned short>(i), chip8_fontset[i]);
  }

  std::ifstream program(file_name.c_str(), std::fstream::binary);
  std::vector<unsigned char> program_vec((std::istreambuf_iterator<char>(program)),
                                         std::istreambuf_iterator<char>());

  // load program into memory
  for (auto i = 0; i < program_vec.size(); i++) {
    memory.set_value(cpu.pc_reg + i, program_vec[i]);
  }
}

void Chip8::emulate_cycle() {
  cpu.fetch(memory);

  cpu.decode_execute(memory, graphics, keypad);

  if (cpu.delay_timer_reg > 0) {
    --cpu.delay_timer_reg;
  }

  if (cpu.sound_timer_reg > 0) {
    if (cpu.sound_timer_reg == 1) {
      // TODO
    }
    --cpu.sound_timer_reg;
  }
}

bool Chip8::should_draw() const { return cpu.updated_screen; }

bool Chip8::get_pixel(unsigned int x, unsigned int y) const { return graphics.get_pixel(x, y); }

bool Chip8::has_next_pixel() {
  return ((curr_x < arch::graphics::screen_width) && (curr_y < arch::graphics::screen_height));
}

