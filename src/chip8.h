#pragma once
#ifndef CHIP8EMULATOR_CHIP8_H_
#  define CHIP8EMULATOR_CHIP8_H_

#  include "arch/cpu.h"
#  include "arch/graphics.h"
#  include "arch/keypad.h"
#  include "arch/memory.h"

class Chip8 {
public:
  Chip8() = default;

  void emulate_cycle();

  bool should_draw();

  void set_keys();

private:
  CPU cpu;
  Memory memory;
  Keypad keypad;
  Graphics graphics
};

#endif  // !CHIP8EMULATOR_CHIP8_H_
