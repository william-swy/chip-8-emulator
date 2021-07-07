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

void Chip8::set_keys(KeyPressed& key_state) {
  // Check zero
  if (key_state.zero_pressed) {
    keypad.press_key(0x0);
  } else {
    keypad.release_key(0x0);
  }

  // Check one
  if (key_state.one_pressed) {
    keypad.press_key(0x1);
  } else {
    keypad.release_key(0x1);
  }

  // Check two
  if (key_state.two_pressed) {
    keypad.press_key(0x2);
  } else {
    keypad.release_key(0x2);
  }

  // Check three
  if (key_state.three_pressed) {
    keypad.press_key(0x3);
  } else {
    keypad.release_key(0x3);
  }

  // Check four
  if (key_state.four_pressed) {
    keypad.press_key(0x4);
  } else {
    keypad.release_key(0x4);
  }

  // Check five
  if (key_state.five_pressed) {
    keypad.press_key(0x5);
  } else {
    keypad.release_key(0x5);
  }

  // Check six
  if (key_state.six_pressed) {
    keypad.press_key(0x6);
  } else {
    keypad.release_key(0x6);
  }

  // Check seven
  if (key_state.seven_pressed) {
    keypad.press_key(0x7);
  } else {
    keypad.release_key(0x7);
  }

  // Check eight
  if (key_state.eight_pressed) {
    keypad.press_key(0x8);
  } else {
    keypad.release_key(0x8);
  }

  // Check nine
  if (key_state.nine_pressed) {
    keypad.press_key(0x9);
  } else {
    keypad.release_key(0x9);
  }

  // Check A
  if (key_state.a_pressed) {
    keypad.press_key(0xA);
  } else {
    keypad.release_key(0xA);
  }

  // Check B
  if (key_state.b_pressed) {
    keypad.press_key(0xB);
  } else {
    keypad.release_key(0xB);
  }

  // Check C
  if (key_state.c_pressed) {
    keypad.press_key(0xC);
  } else {
    keypad.release_key(0xC);
  }

  // Check D
  if (key_state.d_pressed) {
    keypad.press_key(0xD);
  } else {
    keypad.release_key(0xD);
  }

  // Check E
  if (key_state.e_pressed) {
    keypad.press_key(0xE);
  } else {
    keypad.release_key(0xE);
  }

  // Check F
  if (key_state.f_pressed) {
    keypad.press_key(0xF);
  } else {
    keypad.release_key(0xF);
  }
}