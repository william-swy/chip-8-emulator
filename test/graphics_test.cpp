#include "graphics.h"

#include <catch2/catch.hpp>
#include <random>

TEST_CASE("graphics_test_constructor", "[graphics]") {
  const arch::Graphics graphics{};
  for (auto i = 0; i < arch::graphics::screen_width; i++) {
    for (auto j = 0; j < arch::graphics::screen_height; j++) {
      REQUIRE_FALSE(graphics.get_pixel(i, j));
    }
  }
}

TEST_CASE("graphics_test_clear_screen", "[graphics]") {
  arch::Graphics graphics{};

  const std::string seed_str("RNG seed string");
  const std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> pixel_val(0, 1);
  std::uniform_int_distribution<size_t> x_pos(0, arch::graphics::screen_width - 1);
  std::uniform_int_distribution<size_t> y_pos(0, arch::graphics::screen_height - 1);

  for (auto _ = 0; _ < 1000; _++) {
    graphics.set_pixel(x_pos(gen), y_pos(gen), static_cast<bool>(pixel_val(gen)));
  }

  graphics.clear_screen();

  for (auto i = 0; i < arch::graphics::screen_width; i++) {
    for (auto j = 0; j < arch::graphics::screen_height; j++) {
      REQUIRE_FALSE(graphics.get_pixel(i, j));
    }
  }
}

TEST_CASE("graphics_test_set_get_pixel", "[graphics]") {
  arch::Graphics graphics{};

  graphics.set_pixel(12, 20, true);
  REQUIRE(graphics.get_pixel(12, 20));
}

TEST_CASE("graphics_test_get_pixel_x_out_of_bounds", "[graphics]") {
  arch::Graphics graphics{};
  try {
    graphics.get_pixel(arch::graphics::screen_width, 12);
    FAIL("PixelCoordinateOutOfBounds exception should have been thrown\n");
  } catch (const arch::graphics::PixelCoordinateOutOfBounds&) {
    SUCCEED();
  }
}

TEST_CASE("graphics_test_get_pixel_y_out_of_bounds", "[graphics]") {
  arch::Graphics graphics{};
  try {
    graphics.get_pixel(12, arch::graphics::screen_height);
    FAIL("PixelCoordinateOutOfBounds exception should have been thrown\n");
  } catch (const arch::graphics::PixelCoordinateOutOfBounds&) {
    SUCCEED();
  }
}

TEST_CASE("graphics_test_get_pixel_x_and_y_out_of_bounds", "[graphics]") {
  arch::Graphics graphics{};
  try {
    graphics.get_pixel(arch::graphics::screen_width, arch::graphics::screen_height);
    FAIL("PixelCoordinateOutOfBounds exception should have been thrown\n");
  } catch (const arch::graphics::PixelCoordinateOutOfBounds&) {
    SUCCEED();
  }
}

TEST_CASE("graphics_test_set_pixel_x_out_of_bounds", "[graphics]") {
  arch::Graphics graphics{};
  try {
    graphics.set_pixel(arch::graphics::screen_width, 20, true);
    FAIL("PixelCoordinateOutOfBounds exception should have been thrown\n");
  } catch (const arch::graphics::PixelCoordinateOutOfBounds&) {
    SUCCEED();
  }
}

TEST_CASE("graphics_test_set_pixel_y_out_of_bounds", "[graphics]") {
  arch::Graphics graphics{};
  try {
    graphics.set_pixel(15, arch::graphics::screen_height, false);
    FAIL("PixelCoordinateOutOfBounds exception should have been thrown\n");
  } catch (const arch::graphics::PixelCoordinateOutOfBounds&) {
    SUCCEED();
  }
}

TEST_CASE("graphics_test_set_pixel_x_and_y_out_of_bounds", "[graphics]") {
  arch::Graphics graphics{};
  try {
    graphics.set_pixel(arch::graphics::screen_width, arch::graphics::screen_height, true);
    FAIL("PixelCoordinateOutOfBounds exception should have been thrown\n");
  } catch (const arch::graphics::PixelCoordinateOutOfBounds&) {
    SUCCEED();
  }
}

TEST_CASE("graphics_test_draw_pixel_true_true", "[graphics]") {
  arch::Graphics graphics{};
  graphics.set_pixel(2, 5, true);
  REQUIRE(graphics.draw_pixel(2, 5, true));
}

TEST_CASE("graphics_test_draw_pixel_true_false", "[graphics]") {
  arch::Graphics graphics{};
  graphics.set_pixel(2, 5, true);
  REQUIRE_FALSE(graphics.draw_pixel(2, 5, false));
}

TEST_CASE("graphics_test_draw_pixel_false_true", "[graphics]") {
  arch::Graphics graphics{};
  graphics.set_pixel(2, 5, false);
  REQUIRE_FALSE(graphics.draw_pixel(2, 5, true));
}

TEST_CASE("graphics_test_draw_pixel_false_false", "[graphics]") {
  arch::Graphics graphics{};
  graphics.set_pixel(2, 5, false);
  REQUIRE_FALSE(graphics.draw_pixel(2, 5, false));
}