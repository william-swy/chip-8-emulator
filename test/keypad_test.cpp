#include "keypad.h"

#include <catch2/catch.hpp>

TEST_CASE("keypad_test_constructor", "[keypad]") {
  arch::Keypad keypad{};

  for (auto key = 0; key < arch::keypad::num_of_keys; key++) {
    REQUIRE_FALSE(keypad.is_pressed(static_cast<unsigned char>(key)));
  }
}
