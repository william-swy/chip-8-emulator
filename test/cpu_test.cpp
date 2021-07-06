#include "cpu.h"

#include <gtest/gtest.h>

#include <array>
#include <random>

#include "memory.h"

TEST(cpu_test, get_first_general_reg) {
  const arch::CPU cpu;
  try {
    const auto curr_reg = cpu.get_general_reg(0);
    EXPECT_EQ(curr_reg, 0);
  } catch (const arch::InvalidRegisterID&) {
    FAIL() << "InvalidRegisterID exception should not have been thrown\n";
  }
}

TEST(cpu_test, get_last_general_reg) {
  const arch::CPU cpu;
  try {
    const auto curr_reg = cpu.get_general_reg(0xF);
    EXPECT_EQ(curr_reg, 0);
  } catch (const arch::InvalidRegisterID&) {
    FAIL() << "InvalidRegisterID exception should not have been thrown\n";
  }
}

TEST(cpu_test, get_random_general_reg) {
  const arch::CPU cpu;
  const std::string seed_str("Definately a random string");
  std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> random_reg(0, 0xF);

  int curr_iter = 0;
  constexpr int max_iter = 10;

  while (curr_iter < max_iter) {
    const auto curr_reg = static_cast<size_t>(random_reg(gen));
    try {
      const auto result = cpu.get_general_reg(curr_reg);
      EXPECT_EQ(result, 0);
    } catch (const arch::InvalidRegisterID&) {
      FAIL() << "InvalidRegisterID exception should not have been thrown\n";
    }
    curr_iter++;
  }
}

TEST(cpu_test, fail_get_last_reg_out_of_bounds) {
  const arch::CPU cpu;
  try {
    const auto curr_reg = cpu.get_general_reg(arch::num_general_reg);
    FAIL() << "InvalidRegisterID exception should have been thrown\n";
  } catch (const arch::InvalidRegisterID&) {
    SUCCEED();
  }
}

TEST(cpu_test, set_first_general_reg) {
  arch::CPU cpu;
  try {
    constexpr unsigned char expected_val = 0xF1;
    cpu.set_general_reg(0, expected_val);
    const auto curr_reg = cpu.get_general_reg(0);
    EXPECT_EQ(curr_reg, expected_val);
  } catch (const arch::InvalidRegisterID&) {
    FAIL() << "InvalidRegisterID exception should not have been thrown\n";
  }
}

TEST(cpu_test, set_last_general_reg) {
  arch::CPU cpu;
  try {
    constexpr unsigned char expected_val = 0xA3;
    cpu.set_general_reg(0xF, expected_val);
    const auto curr_reg = cpu.get_general_reg(0xF);
    EXPECT_EQ(curr_reg, expected_val);
  } catch (const arch::InvalidRegisterID&) {
    FAIL() << "InvalidRegisterID exception should not have been thrown\n";
  }
}

TEST(cpu_test, set_random_general_reg) {
  arch::CPU cpu;
  const std::string seed_str("Definately a random string");
  std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> random_reg(0, 0xE);
  std::uniform_int_distribution<> random_val(0, 0xFF);

  int curr_iter = 0;
  constexpr int max_iter = 10;

  while (curr_iter < max_iter) {
    const auto curr_reg = static_cast<size_t>(random_reg(gen));
    const auto curr_val = static_cast<unsigned char>(random_val(gen));
    try {
      cpu.set_general_reg(curr_reg, curr_val);
      const auto result = cpu.get_general_reg(curr_reg);
      EXPECT_EQ(result, curr_val);
    } catch (const arch::InvalidRegisterID&) {
      FAIL() << "InvalidRegisterID exception should not have been thrown\n";
    }
    curr_iter++;
  }
}

TEST(cpu_test, fail_set_last_reg_out_of_bounds) {
  arch::CPU cpu;
  try {
    cpu.set_general_reg(arch::num_general_reg, 0x11);
    FAIL() << "InvalidRegisterID exception should have been thrown\n";
  } catch (const arch::InvalidRegisterID&) {
    SUCCEED();
  }
}

TEST(cpu_test, fetch_pc_out_of_bounds) {
  arch::CPU cpu;
  arch::Memory mem;

  cpu.pc_reg = arch::max_mem_address + 1;
  try {
    cpu.fetch(mem);
    FAIL() << "InvalidMemoryAddress exception should have been thrown\n";
  } catch (const arch::InvalidMemoryAddress&) {
    SUCCEED();
  }
}

TEST(cpu_test, fetch_pc_add_one_out_of_bounds) {
  arch::CPU cpu;
  arch::Memory mem;

  cpu.pc_reg = arch::max_mem_address;
  try {
    cpu.fetch(mem);
    FAIL() << "InvalidMemoryAddress exception should have been thrown\n";
  } catch (const arch::InvalidMemoryAddress&) {
    SUCCEED();
  }
}

TEST(cpu_test, fetch_pc_max_address) {
  arch::CPU cpu;
  arch::Memory mem;

  cpu.pc_reg = arch::max_mem_address - 1;
  mem.set_value(arch::max_mem_address, 0xFA);
  mem.set_value(arch::max_mem_address - 1, 0x12);

  try {
    cpu.fetch(mem);
    // REMEMBER: Big Endian
    EXPECT_EQ(cpu.curr_opcode, 0x12FA);
    EXPECT_EQ(cpu.pc_reg, arch::max_mem_address + 1);
  } catch (const arch::InvalidMemoryAddress&) {
    FAIL() << "InvalidMemoryAddress exception should hot have been thrown\n";
  }
}

TEST(cpu_test, fetch_pc_min_address) {
  arch::CPU cpu;
  arch::Memory mem;

  cpu.pc_reg = 0;
  mem.set_value(0, 0x10);
  mem.set_value(1, 0x01);

  try {
    cpu.fetch(mem);
    // REMEMBER: Big Endian
    EXPECT_EQ(cpu.curr_opcode, 0x1001);
    EXPECT_EQ(cpu.pc_reg, 2);
  } catch (const arch::InvalidMemoryAddress&) {
    FAIL() << "InvalidMemoryAddress exception should hot have been thrown\n";
  }
}

TEST(cpu_test, fetch_many_times) {
  struct TestValues {
    unsigned short pc_address;
    unsigned char biggest_value;
    unsigned char smallest_value;
    unsigned short expected_value;
  };

  constexpr size_t size = 10;
  constexpr std::array<TestValues, size> test_vals{{{10, 0xF2, 0x31, 0xF231},
                                                    {100, 0xA3, 0x00, 0xA300},
                                                    {200, 0xC1, 0x29, 0xC129},
                                                    {300, 0xF2, 0x31, 0xF231},
                                                    {1000, 0x52, 0xA1, 0x52A1},
                                                    {1500, 0x90, 0x57, 0x9057},
                                                    {2500, 0x35, 0xAB, 0x35AB},
                                                    {3000, 0x21, 0x32, 0x2132},
                                                    {3500, 0xA7, 0x9A, 0xA79A},
                                                    {4000, 0x8E, 0x22, 0x8E22}}};

  arch::CPU cpu{};
  arch::Memory mem{};

  for (const auto& val : test_vals) {
    cpu.pc_reg = val.pc_address;
    mem.set_value(val.pc_address, val.biggest_value);
    mem.set_value(static_cast<unsigned short>(val.pc_address + 1), val.smallest_value);
    try {
      cpu.fetch(mem);
      EXPECT_EQ(cpu.curr_opcode, val.expected_value);
      EXPECT_EQ(cpu.pc_reg, val.pc_address + 2);
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL() << "InvalidMemoryAddress exception should hot have been thrown\n";
    }
  }
}

TEST(cpu_test, fetch_many_random_times) {
  struct TestInput {
    unsigned char biggest_value;
    unsigned char smallest_value;
    unsigned short expected_value;
  };

  const std::string seed_str("Definately a random string");
  std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> random_address(0, arch::max_mem_address - 1);

  constexpr auto size = 12;

  constexpr std::array<TestInput, size> test_vals{{
      {0xAA, 0xAA, 0xAAAA},
      {0x32, 0xA3, 0x32A3},
      {0x41, 0xEF, 0x41EF},
      {0x38, 0xA2, 0x38A2},
      {0xF1, 0x1F, 0xF11F},
      {0x12, 0x34, 0x1234},
      {0x56, 0x78, 0x5678},
      {0x90, 0xAB, 0x90AB},
      {0xCD, 0xEF, 0xCDEF},
      {0xAB, 0x67, 0xAB67},
      {0xF1, 0x01, 0xF101},
      {0xCE, 0x2A, 0xCE2A},
  }};

  arch::CPU cpu{};
  arch::Memory mem{};

  for (const auto& val : test_vals) {
    const auto pc_address = static_cast<unsigned short>(random_address(gen));
    cpu.pc_reg = pc_address;
    mem.set_value(pc_address, val.biggest_value);
    mem.set_value(static_cast<unsigned short>(pc_address + 1), val.smallest_value);
    try {
      cpu.fetch(mem);
      EXPECT_EQ(cpu.curr_opcode, val.expected_value);
      EXPECT_EQ(cpu.pc_reg, pc_address + 2);
    } catch (const arch::InvalidMemoryAddress&) {
      FAIL() << "InvalidMemoryAddress exception should hot have been thrown\n";
    }
  }
}

TEST(cpu_test, set_stack_pointer_smallest_val) {
  arch::CPU cpu;
  try {
    cpu.set_stack_pointer(0);
    SUCCEED();
  } catch (const arch::InvalidStackPointerValue&) {
    FAIL() << "InvalidStackPointerValue exception should not have been thrown.\n";
  }
}

TEST(cpu_test, set_stack_pointer_biggest_val) {
  arch::CPU cpu;
  try {
    cpu.set_stack_pointer(arch::stack_size - 1);
    SUCCEED();
  } catch (const arch::InvalidStackPointerValue&) {
    FAIL() << "InvalidStackPointerValue exception should not have been thrown.\n";
  }
}

TEST(cpu_test, set_stack_pointer_val_too_big) {
  arch::CPU cpu;
  try {
    cpu.set_stack_pointer(arch::stack_size);
    FAIL() << "InvalidStackPointerValue exception should have been thrown.\n";
  } catch (const arch::InvalidStackPointerValue&) {
    SUCCEED();
  }
}
