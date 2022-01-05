#include "memory.h"

#include <catch2/catch.hpp>

#include <array>
#include <random>
#include <tuple>

TEST_CASE("memory_set_get_single_value", "[memory]") {
  arch::Memory test_mem;
  constexpr unsigned short test_address = 100;
  constexpr unsigned char expected_val = 0x10;
  try {
    test_mem.set_value(test_address, expected_val);
    try {
      const unsigned char result = test_mem.get_value(test_address);
      REQUIRE(result == expected_val);
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
    }
  } catch (const arch::InvalidMemoryAddress&) {
    FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
  }
}

TEST_CASE("memory_set_get_same_address_many_times", "[memory]") {
  arch::Memory test_mem;
  constexpr size_t size = 10;
  constexpr unsigned short test_address = 450;
  constexpr std::array<unsigned char, size> values
      = {0x30, 0x35, 0x3a, 0x3f, 0x65, 0x6a, 0x6f, 0x95, 0x9f, 0x9a};

  for (const auto& val : values) {
    try {
      test_mem.set_value(test_address, val);
      try {
        const unsigned char result = test_mem.get_value(test_address);
        REQUIRE(result == val);
      } catch (const arch::InvalidMemoryAddress&) {
        FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
      }
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
    }
  }
}

TEST_CASE("memory_set_get_diff_address_many_times", "[memory]") {
  arch::Memory test_mem;

  constexpr size_t size = 10;
  // First element of tuple is address and second element of tuple is the value inserted at the
  // address. Somewhat cursed just to get compiler to be silent about tuple type conversion from
  // constructor.
  constexpr std::array<std::tuple<unsigned short, unsigned char>, size> address_values
      = {std::make_tuple(static_cast<unsigned short>(150), static_cast<unsigned char>(0x20)),
         std::make_tuple(static_cast<unsigned short>(200), static_cast<unsigned char>(0x30)),
         std::make_tuple(static_cast<unsigned short>(250), static_cast<unsigned char>(0x40)),
         std::make_tuple(static_cast<unsigned short>(300), static_cast<unsigned char>(0x50)),
         std::make_tuple(static_cast<unsigned short>(350), static_cast<unsigned char>(0x60)),
         std::make_tuple(static_cast<unsigned short>(400), static_cast<unsigned char>(0x70)),
         std::make_tuple(static_cast<unsigned short>(450), static_cast<unsigned char>(0x80)),
         std::make_tuple(static_cast<unsigned short>(500), static_cast<unsigned char>(0x90)),
         std::make_tuple(static_cast<unsigned short>(550), static_cast<unsigned char>(0xA0)),
         std::make_tuple(static_cast<unsigned short>(600), static_cast<unsigned char>(0xB0))};

  for (const auto& value : address_values) {
    const auto& [test_address, expected_val] = value;
    try {
      test_mem.set_value(test_address, expected_val);
      try {
        const unsigned char result = test_mem.get_value(test_address);
        REQUIRE(result == expected_val);
      } catch (const arch::InvalidMemoryAddress&) {
        FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
      }
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
    }
  }
}

TEST_CASE("memory_set_get_smallest_index", "[memory]") {
  arch::Memory test_mem;
  constexpr unsigned short test_address = 0;
  constexpr unsigned char expected_val = 0xFF;
  try {
    test_mem.set_value(test_address, expected_val);
    try {
      const unsigned char result = test_mem.get_value(test_address);
      REQUIRE(result == expected_val);
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
    }
  } catch (const arch::InvalidMemoryAddress&) {
    FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
  }
}

TEST_CASE("memory_set_get_biggest_index", "[memory]") {
  arch::Memory test_mem;
  constexpr unsigned short test_address = arch::max_mem_address;
  constexpr unsigned char expected_val = 0xA1;
  try {
    test_mem.set_value(test_address, expected_val);
    try {
      const unsigned char result = test_mem.get_value(test_address);
      REQUIRE(result == expected_val);
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
    }
  } catch (const arch::InvalidMemoryAddress&) {
    FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
  }
}

TEST_CASE("memory_set_get_many_random_indicies", "[memory]") {
  const std::string seed_str("Definately a random string");
  std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);  // seed the generator
  std::uniform_int_distribution<> random_address(0, arch::max_mem_address);
  std::uniform_int_distribution<> random_value(0x0, 0xFF);

  constexpr size_t num_elem = 1000;
  std::array<std::tuple<unsigned short, unsigned char>, num_elem> address_vals = {};

  for (size_t idx = 0; idx < address_vals.size(); idx++) {
    address_vals[idx] = std::make_tuple(static_cast<unsigned short>(random_address(gen)),
                                        static_cast<unsigned char>(random_value(gen)));
  }

  arch::Memory test_mem;

  for (const auto& value : address_vals) {
    const auto& [test_address, expected_val] = value;
    try {
      test_mem.set_value(test_address, expected_val);
      try {
        const unsigned char result = test_mem.get_value(test_address);
        REQUIRE(result == expected_val);
      } catch (const arch::InvalidMemoryAddress&) {
        FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
      }
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL("InvalidMemoryAddress exception should not have been thrown.\n");
    }
  }
}

TEST_CASE("memory_fail_get_memory_index_one_greater_than_max", "[memory]") {
  constexpr arch::Memory test_mem{};
  constexpr unsigned short test_address = arch::mem_size;
  try {
    const unsigned char result = test_mem.get_value(test_address);
    FAIL("InvalidMemoryAddress exception should have been thrown\n") ;

  } catch (const arch::InvalidMemoryAddress&) {
    SUCCEED();
  }
}

TEST_CASE("memory_fail_get_many_random_indicies_out_of_bounds", "[memory]") {
  const std::string seed_str("Definately a random string");
  std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);  // seed the generator
  std::uniform_int_distribution<> random_address(arch::mem_size, 0xFFFF);

  constexpr arch::Memory test_mem{};

  int count = 0;
  constexpr int iterations = 1000;
  while (count < iterations) {
    const auto test_address = static_cast<unsigned short>(random_address(gen));

    try {
      const unsigned char result = test_mem.get_value(test_address);
      FAIL("InvalidMemoryAddress exception should have been thrown\n") ;

    } catch (const arch::InvalidMemoryAddress&) {
      SUCCEED();
    }
    count++;
  }
}

TEST_CASE("memory_fail_set_memory_index_one_greater_than_max", "[memory]") {
  arch::Memory test_mem;
  constexpr unsigned char expected_val = 0xFF;
  try {
    test_mem.set_value(arch::mem_size, expected_val);
    FAIL("InvalidMemoryAddress exception should have been thrown.\n");  
  } catch (const arch::InvalidMemoryAddress&) {
    SUCCEED();
  }
}

TEST_CASE("memory_fail_set_many_random_indicies_out_of_bounds", "[memory]") {
  const std::string seed_str("Definately a random string");
  std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);  // seed the generator
  std::uniform_int_distribution<> random_address(arch::mem_size, 0xFFFF);

  arch::Memory test_mem{};

  int count = 0;
  constexpr int iterations = 1000;
  while (count < iterations) {
    const auto test_address = static_cast<unsigned short>(random_address(gen));

    try {
      test_mem.set_value(test_address, 0x10);
      FAIL("InvalidMemoryAddress exception should have been thrown\n") ;

    } catch (const arch::InvalidMemoryAddress&) {
      SUCCEED();
    }
    count++;
  }
}