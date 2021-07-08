#pragma once
#ifndef CHIP8EMULATOR_ARCH_KEYPAD_H_
#  define CHIP8EMULATOR_ARCH_KEYPAD_H_

#  include <array>
#  include <stdexcept>

namespace arch {
  namespace keypad {
    constexpr size_t num_of_keys = 16;  // Number of keys

    class InvalidKey : public std::exception {
    public:
      virtual const char* what() const noexcept { return "Invalid key.\n"; }
    };
  }  // namespace keypad
  class Keypad {
  public:
    Keypad();

    void press_key(unsigned char key_num);

    void release_key(unsigned char key_num);

    [[nodiscard]] bool is_pressed(unsigned char key_num) const;

  private:
    std::array<bool, keypad::num_of_keys> keys_state;  // State of each key
  };
}  // namespace arch
#endif  // !CHIP8EMULATOR_ARCH_KEYPAD_H_
