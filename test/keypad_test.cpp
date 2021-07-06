#include "keypad.h"

#include <gtest/gtest.h>

TEST(keypad_test, test_constructor) {
  arch::Keypad keypad{};

  for (auto key = 0; key < arch::keypad::num_of_keys; key++) {
    EXPECT_EQ(keypad.is_pressed(static_cast<unsigned char>(key)), false);
  }
}
