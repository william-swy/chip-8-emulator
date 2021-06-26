#pragma once
#ifndef CHIP8EMULATOR_ARCH_KEYPAD_H_
#  define CHIP8EMULATOR_ARCH_KEYPAD_H_

#  include <array>

namespace arch {
  class Keypad {
  public:
    std::array<unsigned char, num_of_keys> keys_state;  // Stete of each key

  private:
    constexpr usize num_of_keys = 16;  // Number of keys
  };
}  // namespace arch
#endif  // !CHIP8EMULATOR_ARCH_KEYPAD_H_
