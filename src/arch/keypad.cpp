#include "keypad.h"

arch::Keypad::Keypad() { keys_state.fill(false); }

void arch::Keypad::press_key(unsigned char key_num) {
  if (key_num >= arch::keypad::num_of_keys) {
    throw arch::keypad::InvalidKey();
  } else {
    keys_state[key_num] = true;
    key_pressed = true;
    pressed_key = key_num;
  }
}

void arch::Keypad::release_key(unsigned char key_num) {
  if (key_num >= arch::keypad::num_of_keys) {
    throw arch::keypad::InvalidKey();
  } else {
    keys_state[key_num] = false;
    key_pressed = false;
  }
}

bool arch::Keypad::is_pressed(unsigned char key_num) const {
  if (key_num >= arch::keypad::num_of_keys) {
    throw arch::keypad::InvalidKey();
  } else {
    return keys_state[key_num];
  }
}
