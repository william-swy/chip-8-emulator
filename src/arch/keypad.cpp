#include "keypad.h"

arch::Keypad::Keypad() { keys_state.fill(false); }

void arch::Keypad::press_key(unsigned char key_num) {
  try {
    keys_state.at(key_num) = true;
    key_pressed = true;
    pressed_key = key_num;
  } catch (const std::out_of_range&) {
    throw arch::keypad::InvalidKey();
  }
}

void arch::Keypad::release_key(unsigned char key_num) {
  try {
    keys_state.at(key_num) = false;
    key_pressed = false;
  } catch (const std::out_of_range&) {
    throw arch::keypad::InvalidKey();
  }
}

bool arch::Keypad::is_pressed(unsigned char key_num) const {
  try {
    return keys_state.at(key_num);
  } catch (const std::out_of_range&) {
    throw arch::keypad::InvalidKey();
  }
}
