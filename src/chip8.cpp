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
    memory.set_value(static_cast<unsigned short>(cpu.pc_reg + i), program_vec[i]);
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

void Chip8::handle_keys(enum input_events::Events key_state) {
  // A nasty switch
  switch (key_state) {
    case input_events::Events::zero_pressed:
      keypad.press_key(0x0);
      break;
    case input_events::Events::one_pressed:
      keypad.press_key(0x1);
      break;
    case input_events::Events::two_pressed:
      keypad.press_key(0x2);
      break;
    case input_events::Events::three_pressed:
      keypad.press_key(0x3);
      break;
    case input_events::Events::four_pressed:
      keypad.press_key(0x4);
      break;
    case input_events::Events::five_pressed:
      keypad.press_key(0x5);
      break;
    case input_events::Events::six_pressed:
      keypad.press_key(0x6);
      break;
    case input_events::Events::seven_pressed:
      keypad.press_key(0x7);
      break;
    case input_events::Events::eight_pressed:
      keypad.press_key(0x8);
      break;
    case input_events::Events::nine_pressed:
      keypad.press_key(0x9);
      break;
    case input_events::Events::a_pressed:
      keypad.press_key(0xA);
      break;
    case input_events::Events::b_pressed:
      keypad.press_key(0xB);
      break;
    case input_events::Events::c_pressed:
      keypad.press_key(0xC);
      break;
    case input_events::Events::d_pressed:
      keypad.press_key(0xD);
      break;
    case input_events::Events::e_pressed:
      keypad.press_key(0xE);
      break;
    case input_events::Events::f_pressed:
      keypad.press_key(0xF);
      break;
    case input_events::Events::zero_released:
      keypad.release_key(0x0);
      break;
    case input_events::Events::one_released:
      keypad.release_key(0x1);
      break;
    case input_events::Events::two_released:
      keypad.release_key(0x2);
      break;
    case input_events::Events::three_released:
      keypad.release_key(0x3);
      break;
    case input_events::Events::four_released:
      keypad.release_key(0x4);
      break;
    case input_events::Events::five_released:
      keypad.release_key(0x5);
      break;
    case input_events::Events::six_released:
      keypad.release_key(0x6);
      break;
    case input_events::Events::seven_released:
      keypad.release_key(0x7);
      break;
    case input_events::Events::eight_released:
      keypad.release_key(0x8);
      break;
    case input_events::Events::nine_released:
      keypad.release_key(0x9);
      break;
    case input_events::Events::a_released:
      keypad.release_key(0xA);
      break;
    case input_events::Events::b_released:
      keypad.release_key(0xB);
      break;
    case input_events::Events::c_released:
      keypad.release_key(0xC);
      break;
    case input_events::Events::d_released:
      keypad.release_key(0xD);
      break;
    case input_events::Events::e_released:
      keypad.release_key(0xE);
      break;
    case input_events::Events::f_released:
      keypad.release_key(0xF);
      break;
    default:
      break;
  }
}