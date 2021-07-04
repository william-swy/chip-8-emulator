#include <gtest/gtest.h>

#include <array>
#include <random>

#include "arch/cpu.h"
#include "arch/graphics.h"
#include "arch/keypad.h"
#include "arch/memory.h"

TEST(cpu_opcode_test, execute_instruction_ANNN_once) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xA123;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.index_reg, 0x0123);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_ANNN_many_times) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  struct OpcodeExpected {
    unsigned short opcode;
    unsigned short expected;
  };

  constexpr size_t size = 10;
  constexpr std::array<OpcodeExpected, size> opcodes{{{0xAA3D, 0x0A3D},
                                                      {0xA156, 0x0156},
                                                      {0xAABC, 0x0ABC},
                                                      {0xABBB, 0x0BBB},
                                                      {0xAC12, 0x0C12},
                                                      {0xA980, 0x0980},
                                                      {0xAE46, 0x0E46},
                                                      {0xAF2F, 0x0F2F},
                                                      {0xA45A, 0x045A},
                                                      {0xAD37, 0x0D37}}};

  for (const auto& val : opcodes) {
    cpu.curr_opcode = val.opcode;
    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.index_reg, val.expected);
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_BNNN_once) {
  arch::CPU cpu;
  arch::Memory mem;
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.set_general_reg(0, 0xF1);
  cpu.curr_opcode = 0xB234;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x0234 + 0xF1);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_BNNN_many_times) {
  struct TestValues {
    unsigned char reg_v0;
    unsigned short opcode;
    unsigned short expected_address;
  };

  constexpr std::array<TestValues, 10> input{{{0x12, 0xBF31, 0x0F31 + 0x12},
                                              {0xE7, 0xB45A, 0x045A + 0xE7},
                                              {0xBB, 0xB3EF, 0x03EF + 0xBB},
                                              {0xF8, 0xB345, 0x0345 + 0xF8},
                                              {0x42, 0xB809, 0x0809 + 0x42},
                                              {0xA4, 0xB467, 0x0467 + 0xA4},
                                              {0x60, 0xB4E8, 0x04E8 + 0x60},
                                              {0x3C, 0xBCD1, 0x0CD1 + 0x3C},
                                              {0xFF, 0xBE0F, 0x0E0F + 0xFF},
                                              {0x55, 0xB39A, 0x039A + 0x55}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto val : input) {
    cpu.set_general_reg(0, val.reg_v0);
    cpu.curr_opcode = val.opcode;

    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.pc_reg, val.expected_address

      );
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_1NNN_once) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xB123;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x0123);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_1NNN_many_times) {
  struct TestValues {
    unsigned short opcode;
    unsigned short expected_address;
  };

  constexpr std::array<TestValues, 10> input{{{0x1A12, 0x0A12},
                                              {0x1E7F, 0x0E7F},
                                              {0x134D, 0x034D},
                                              {0x1AF8, 0x0AF8},
                                              {0x1111, 0x0111},
                                              {0x1010, 0x0010},
                                              {0x1FFF, 0x0FFF},
                                              {0x1CDE, 0x0CDE},
                                              {0x15F2, 0x05F2},
                                              {0x1ABC, 0x0ABC}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : input) {
    try {
      cpu.curr_opcode = val.opcode;
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.pc_reg, val.expected_address);
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_3XNN_once_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x0, 0x11);
  cpu.curr_opcode = 0x3012;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x3460);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_3XNN_once_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x4, 0x25);
  cpu.curr_opcode = 0x3425;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x3460 + 2);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_3XNN_many_assorted_skips) {
  struct TestInputs {
    unsigned short start_pc;
    unsigned short opcode;
    size_t register_id;
    unsigned char register_value;
    bool should_skip;
  };

  constexpr std::array<TestInputs, 10> input_vals{{{0x2100, 0x3145, 0x0, 0x12, false},
                                                   {0x3456, 0x33FF, 0x3, 0xFF, true},
                                                   {0xF123, 0x3A11, 0xA, 0x4E, false},
                                                   {0xABCD, 0x3C3C, 0x1, 0x11, false},
                                                   {0x78AC, 0x35AA, 0x5, 0xAA, true},
                                                   {0xB78A, 0x39F1, 0x9, 0xF1, true},
                                                   {0x74B3, 0x3BB1, 0xB, 0xB1, true},
                                                   {0x5189, 0x3743, 0x7, 0xA6, false},
                                                   {0xAB80, 0x385A, 0x7, 0x5A, false},
                                                   {0x6520, 0x3871, 0x0, 0x71, false}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : input_vals) {
    cpu.pc_reg = val.start_pc;
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.register_id, val.register_value);
    try {
      cpu.decode_execute(mem, graphics, keypad);
      if (val.should_skip) {
        EXPECT_EQ(cpu.pc_reg, val.start_pc + 2);
      } else {
        EXPECT_EQ(cpu.pc_reg, val.start_pc);
      }
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown.\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_4XNN_once_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x0, 0x12);
  cpu.curr_opcode = 0x4012;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x3460);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_4XNN_once_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x4, 0xAF);
  cpu.curr_opcode = 0x4425;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x3460 + 2);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_4XNN_many_assorted_skips) {
  struct TestInputs {
    unsigned short start_pc;
    unsigned short opcode;
    size_t register_id;
    unsigned char register_value;
    bool should_skip;
  };

  constexpr std::array<TestInputs, 10> input_vals{{{0x2100, 0x4145, 0x0, 0x12, true},
                                                   {0x3456, 0x43FF, 0x3, 0xFF, false},
                                                   {0xF123, 0x4A11, 0xA, 0x4E, true},
                                                   {0xABCD, 0x4C3C, 0x1, 0x11, true},
                                                   {0x78AC, 0x45AA, 0x5, 0xAA, false},
                                                   {0xB78A, 0x49F1, 0x9, 0xF1, false},
                                                   {0x74B3, 0x4BB1, 0xB, 0xB1, false},
                                                   {0x5189, 0x4743, 0x7, 0xA6, true},
                                                   {0xAB80, 0x485A, 0x7, 0x5A, true},
                                                   {0x6520, 0x4871, 0x0, 0x71, true}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : input_vals) {
    cpu.pc_reg = val.start_pc;
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.register_id, val.register_value);
    try {
      cpu.decode_execute(mem, graphics, keypad);
      if (val.should_skip) {
        EXPECT_EQ(cpu.pc_reg, val.start_pc + 2);
      } else {
        EXPECT_EQ(cpu.pc_reg, val.start_pc);
      }
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown.\n";
    }
  }
}

TEST(cpu_opcode_test, invalid_5000_instruction) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x53F1;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    FAIL() << "InvalidInstruction exception should have been thrown.\n";
  } catch (const arch::InvalidInstruction&) {
    SUCCEED();
  }
}

TEST(cpu_opcode_test, execute_instruction_5XY0_once_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x57F0;
  cpu.set_general_reg(0x7, 0x3);
  cpu.set_general_reg(0xF, 0x2);
  cpu.pc_reg = 0x300;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x300);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_5XY0_once_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x54B0;
  cpu.set_general_reg(0x4, 0xF);
  cpu.set_general_reg(0xB, 0xF);
  cpu.pc_reg = 0x512;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x512 + 0x2);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_5XY0_many_assorted_skips) {
  struct TestInputs {
    unsigned short opcode;
    unsigned short curr_pc;
    size_t reg_X;
    size_t reg_Y;
    unsigned char reg_X_val;
    unsigned char reg_Y_val;
    bool should_skip;
  };

  constexpr std::array<TestInputs, 10> inputs{{{0x5120, 0x3456, 0x1, 0x2, 0x12, 0x12, true},
                                               {0x5FF0, 0x3450, 0xF, 0xF, 0xF3, 0xF3, true},
                                               {0x58A0, 0x67F2, 0x3, 0xA, 0x12, 0x4C, false},
                                               {0x5890, 0x0000, 0x8, 0x9, 0x3D, 0x3D, true},
                                               {0x5C00, 0x3786, 0xC, 0x0, 0x79, 0x01, false},
                                               {0x5B40, 0x38AA, 0xB, 0x4, 0x12, 0xAB, false},
                                               {0x5010, 0x3478, 0x0, 0x1, 0xFB, 0x6E, false},
                                               {0x57D0, 0x6780, 0x7, 0xD, 0x2A, 0x2A, true},
                                               {0x5830, 0x4568, 0x8, 0x3, 0x12, 0x13, false},
                                               {0x5E40, 0x876A, 0xE, 0x4, 0xCE, 0xCE, true}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : inputs) {
    cpu.curr_opcode = val.opcode;
    cpu.pc_reg = val.curr_pc;
    cpu.set_general_reg(val.reg_X, val.reg_X_val);
    cpu.set_general_reg(val.reg_Y, val.reg_Y_val);
    try {
      cpu.decode_execute(mem, graphics, keypad);
      if (val.should_skip) {
        EXPECT_EQ(cpu.pc_reg, val.curr_pc + 2);
      } else {
        EXPECT_EQ(cpu.pc_reg, val.curr_pc);
      }

    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown.\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_6XNN_once) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x6123;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0x1), 0x23);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_6XNN_many_times) {
  struct TestInputs {
    unsigned short opcode;
    size_t reg_id;
    unsigned char value;
  };

  constexpr std::array<TestInputs, 10> inputs{{
      {0x61FF, 0x1, 0xFF},
      {0x6212, 0x2, 0x12},
      {0x63AC, 0x3, 0xAC},
      {0x6430, 0x4, 0x30},
      {0x659A, 0x5, 0x9A},
      {0x6600, 0x6, 0x00},
      {0x6ABF, 0xA, 0xBF},
      {0x6BD1, 0xB, 0xD1},
      {0x6C05, 0xC, 0x05},
      {0x6D5A, 0xD, 0x5A},
  }};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : inputs) {
    cpu.curr_opcode = val.opcode;

    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.get_general_reg(val.reg_id), val.value);
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown.\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_7XNN_once) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x70F1;
  cpu.set_general_reg(0x0, 0x10);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0x0), static_cast<unsigned char>(0x10 + 0xF1));
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_7XNN_many_times) {
  struct TestInputs {
    unsigned short opcode;
    size_t reg_id;
    unsigned char initial_val;
    unsigned char add_value;
  };

  constexpr std::array<TestInputs, 10> inputs{{
      {0x71FF, 0x1, 0xAB, 0xFF},
      {0x7212, 0x2, 0x11, 0x12},
      {0x73AC, 0x3, 0xC0, 0xAC},
      {0x7430, 0x4, 0x71, 0x30},
      {0x759A, 0x5, 0x89, 0x9A},
      {0x7600, 0x6, 0x00, 0x00},
      {0x7ABF, 0xA, 0x12, 0xBF},
      {0x7BD1, 0xB, 0x34, 0xD1},
      {0x7C05, 0xC, 0x78, 0x05},
      {0x7D5A, 0xD, 0xEA, 0x5A},
  }};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : inputs) {
    cpu.curr_opcode = val.opcode;

    try {
      cpu.set_general_reg(val.reg_id, val.initial_val);
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.get_general_reg(val.reg_id),
                static_cast<unsigned char>(val.initial_val + val.add_value));
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown.\n";
    }
  }
}

TEST(cpu_opcode_test, invalid_9000_instruction) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x93F1;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    FAIL() << "InvalidInstruction exception should have been thrown.\n";
  } catch (const arch::InvalidInstruction&) {
    SUCCEED();
  }
}

TEST(cpu_opcode_test, execute_instruction_9XY0_once_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x97F0;
  cpu.set_general_reg(0x7, 0x3);
  cpu.set_general_reg(0xF, 0x3);
  cpu.pc_reg = 0x300;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x300);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_9XY0_once_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x94B0;
  cpu.set_general_reg(0x4, 0xF);
  cpu.set_general_reg(0xB, 0xA);
  cpu.pc_reg = 0x512;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x512 + 0x2);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_9XY0_many_assorted_skips) {
  struct TestInputs {
    unsigned short opcode;
    unsigned short curr_pc;
    size_t reg_X;
    size_t reg_Y;
    unsigned char reg_X_val;
    unsigned char reg_Y_val;
    bool should_skip;
  };

  constexpr std::array<TestInputs, 10> inputs{{{0x9120, 0x3456, 0x1, 0x2, 0x12, 0x12, false},
                                               {0x9FF0, 0x3450, 0xF, 0xF, 0xF3, 0xF3, false},
                                               {0x98A0, 0x67F2, 0x3, 0xA, 0x12, 0x4C, true},
                                               {0x9890, 0x0000, 0x8, 0x9, 0x3D, 0x3D, false},
                                               {0x9C00, 0x3786, 0xC, 0x0, 0x79, 0x01, true},
                                               {0x9B40, 0x38AA, 0xB, 0x4, 0x12, 0xAB, true},
                                               {0x9010, 0x3478, 0x0, 0x1, 0xFB, 0x6E, true},
                                               {0x97D0, 0x6780, 0x7, 0xD, 0x2A, 0x2A, false},
                                               {0x9830, 0x4568, 0x8, 0x3, 0x12, 0x13, true},
                                               {0x9E40, 0x876A, 0xE, 0x4, 0xCE, 0xCE, false}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : inputs) {
    cpu.curr_opcode = val.opcode;
    cpu.pc_reg = val.curr_pc;
    cpu.set_general_reg(val.reg_X, val.reg_X_val);
    cpu.set_general_reg(val.reg_Y, val.reg_Y_val);
    try {
      cpu.decode_execute(mem, graphics, keypad);
      if (val.should_skip) {
        EXPECT_EQ(cpu.pc_reg, val.curr_pc + 2);
      } else {
        EXPECT_EQ(cpu.pc_reg, val.curr_pc);
      }

    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown.\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_CXNN_once) {
  // We can mimic the random number generator
  const std::string seed_str("RNG seed string");
  const std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> rng(0x00, 0xFF);

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xC1FF;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(1), static_cast<unsigned char>(rng(gen) & 0xFF));

  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_CXNN_many_times) {
  // We can mimic the random number generator
  const std::string seed_str("RNG seed string");
  const std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> rng(0x00, 0xFF);

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  struct TestInputs {
    unsigned short opcode;
    size_t reg_id;
    unsigned char mask;
  };

  constexpr std::array<TestInputs, 5> inputs{{{0xC312, 0x3, 0x12},
                                              {0xC6F1, 0x6, 0xF1},
                                              {0xC94E, 0x9, 0x4E},
                                              {0xCC52, 0xC, 0x52},
                                              {0xCF90, 0xF, 0x90}}};

  for (const auto& val : inputs) {
    cpu.curr_opcode = val.opcode;

    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.get_general_reg(val.reg_id), static_cast<unsigned char>(rng(gen) & val.mask));

    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown.\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_2NNN_once) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x24F2;
  cpu.pc_reg = 0x200;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x04F2);
    EXPECT_EQ(cpu.get_stack_pointer(), 1);
    EXPECT_EQ(cpu.get_stack(), 0x200);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_2NNN_many_times) {
  // Simulates starting in the main function which then calls foo1 which calls bar which then calls
  // foo2

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  constexpr auto start_pc = 0x200;
  constexpr auto call_foo1_opcode = 0x2400;

  constexpr auto foo1_address = 0x400;
  constexpr auto call_bar_opcode = 0x2F42;

  constexpr auto bar_address = 0xF42;
  constexpr auto call_foo2_opcode = 0x2112;

  constexpr auto foo2_address = 0x112;

  cpu.pc_reg = start_pc;
  cpu.curr_opcode = call_foo1_opcode;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, foo1_address);
    EXPECT_EQ(cpu.get_stack_pointer(), 1);
    EXPECT_EQ(cpu.get_stack(), start_pc);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }

  cpu.curr_opcode = call_bar_opcode;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, bar_address);
    EXPECT_EQ(cpu.get_stack_pointer(), 2);
    cpu.set_stack_pointer(1);
    EXPECT_EQ(cpu.get_stack(), start_pc);
    cpu.set_stack_pointer(2);
    EXPECT_EQ(cpu.get_stack(), foo1_address);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }

  cpu.curr_opcode = call_foo2_opcode;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, foo2_address);
    EXPECT_EQ(cpu.get_stack_pointer(), 3);
    cpu.set_stack_pointer(1);
    EXPECT_EQ(cpu.get_stack(), start_pc);
    cpu.set_stack_pointer(2);
    EXPECT_EQ(cpu.get_stack(), foo1_address);
    cpu.set_stack_pointer(3);
    EXPECT_EQ(cpu.get_stack(), bar_address);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_00EE_once) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x00EE;

  cpu.set_stack_pointer(1);
  cpu.set_stack(0x1234);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, 0x1234);
    EXPECT_EQ(cpu.get_stack_pointer(), 0);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_00EE_many_times) {
  // Aims to mock the returning from the call from main to foo1 to bar to foo2 i.e. foo2 returns to
  // bar which returns to foo1 which returns to main.

  constexpr auto opcode = 0x00EE;

  constexpr auto main_return_address = 0x300;
  constexpr auto foo1_return_adress = 0x412;
  constexpr auto bar_return_adress = 0x350;

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.set_stack_pointer(1);
  cpu.set_stack(main_return_address);
  cpu.set_stack_pointer(2);
  cpu.set_stack(foo1_return_adress);
  cpu.set_stack_pointer(3);
  cpu.set_stack(bar_return_adress);

  cpu.curr_opcode = opcode;

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, bar_return_adress);
    EXPECT_EQ(cpu.get_stack_pointer(), 2);
    EXPECT_EQ(cpu.get_stack(), foo1_return_adress);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, foo1_return_adress);
    EXPECT_EQ(cpu.get_stack_pointer(), 1);
    EXPECT_EQ(cpu.get_stack(), main_return_address);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.pc_reg, main_return_address);
    EXPECT_EQ(cpu.get_stack_pointer(), 0);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY0_many_times) {
  struct TestInput {
    unsigned short opcode;
    size_t reg_x;
    size_t reg_y;
    unsigned char reg_x_val;
    unsigned char reg_y_val;
  };

  constexpr std::array<TestInput, 5> input{{{0x8120, 0x1, 0x2, 0x1, 0x2},
                                            {0x8A30, 0xA, 0x3, 0xA, 0xE},
                                            {0x8EF0, 0xE, 0xF, 0xF, 0xF},
                                            {0x8900, 0x9, 0x0, 0x0, 0x5},
                                            {0x8C30, 0xC, 0x3, 0x4, 0x8}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : input) {
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.reg_x, val.reg_x_val);
    cpu.set_general_reg(val.reg_y, val.reg_y_val);

    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.get_general_reg(val.reg_x), val.reg_y_val);
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY1_many_times) {
  struct TestInput {
    unsigned short opcode;
    size_t reg_x;
    size_t reg_y;
    unsigned char reg_x_val;
    unsigned char reg_y_val;
  };

  constexpr std::array<TestInput, 5> input{{{0x8121, 0x1, 0x2, 0x1, 0x2},
                                            {0x8A31, 0xA, 0x3, 0xA, 0xE},
                                            {0x8EF1, 0xE, 0xF, 0xF, 0xF},
                                            {0x8901, 0x9, 0x0, 0x0, 0x5},
                                            {0x8C31, 0xC, 0x3, 0x4, 0x8}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : input) {
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.reg_x, val.reg_x_val);
    cpu.set_general_reg(val.reg_y, val.reg_y_val);

    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.get_general_reg(val.reg_x),
                static_cast<unsigned char>(val.reg_y_val | val.reg_x_val));
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY2_many_times) {
  struct TestInput {
    unsigned short opcode;
    size_t reg_x;
    size_t reg_y;
    unsigned char reg_x_val;
    unsigned char reg_y_val;
  };

  constexpr std::array<TestInput, 5> input{{{0x8122, 0x1, 0x2, 0x1, 0x2},
                                            {0x8A32, 0xA, 0x3, 0xA, 0xE},
                                            {0x8EF2, 0xE, 0xF, 0xF, 0xF},
                                            {0x8902, 0x9, 0x0, 0x0, 0x5},
                                            {0x8C32, 0xC, 0x3, 0x4, 0x8}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : input) {
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.reg_x, val.reg_x_val);
    cpu.set_general_reg(val.reg_y, val.reg_y_val);

    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.get_general_reg(val.reg_x),
                static_cast<unsigned char>(val.reg_y_val & val.reg_x_val));
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY3_many_times) {
  struct TestInput {
    unsigned short opcode;
    size_t reg_x;
    size_t reg_y;
    unsigned char reg_x_val;
    unsigned char reg_y_val;
  };

  constexpr std::array<TestInput, 5> input{{{0x8123, 0x1, 0x2, 0x1, 0x2},
                                            {0x8A33, 0xA, 0x3, 0xA, 0xE},
                                            {0x8EF3, 0xE, 0xF, 0xF, 0xF},
                                            {0x8903, 0x9, 0x0, 0x0, 0x5},
                                            {0x8C33, 0xC, 0x3, 0x4, 0x8}}};

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  for (const auto& val : input) {
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.reg_x, val.reg_x_val);
    cpu.set_general_reg(val.reg_y, val.reg_y_val);

    try {
      cpu.decode_execute(mem, graphics, keypad);
      EXPECT_EQ(cpu.get_general_reg(val.reg_x),
                static_cast<unsigned char>(val.reg_y_val ^ val.reg_x_val));
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY4_no_overflow) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x8A14;

  cpu.set_general_reg(0xA, 0x34);
  cpu.set_general_reg(0x1, 0x12);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0xA), static_cast<unsigned char>(0x34 + 0x12));
    EXPECT_EQ(cpu.get_general_reg(0x1), 0x12);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x0);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY4_overflow) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x83C4;

  cpu.set_general_reg(0x3, 0xFF);
  cpu.set_general_reg(0xC, 0x11);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0x3), static_cast<unsigned char>(0xFF + 0x11));
    EXPECT_EQ(cpu.get_general_reg(0xC), 0x11);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x1);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY5_no_overflow) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x8A15;

  cpu.set_general_reg(0xA, 0x34);
  cpu.set_general_reg(0x1, 0x12);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0xA), static_cast<unsigned char>(0x34 - 0x12));
    EXPECT_EQ(cpu.get_general_reg(0x1), 0x12);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x1);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY5_overflow) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x83C5;

  cpu.set_general_reg(0x3, 0xA1);
  cpu.set_general_reg(0xC, 0xAB);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0x3), static_cast<unsigned char>(0xA1 - 0xAB));
    EXPECT_EQ(cpu.get_general_reg(0xC), 0xAB);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x0);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY6_rightmost_zero) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x8A16;

  cpu.set_general_reg(0x1, 0x14);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0xA), 0xA);
    EXPECT_EQ(cpu.get_general_reg(0x1), 0x14);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x0);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY5_rightmost_one) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x83C6;

  cpu.set_general_reg(0xC, 0xAB);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0x3), 0x55);
    EXPECT_EQ(cpu.get_general_reg(0xC), 0xAB);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x1);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY7_no_overflow) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x8A17;

  cpu.set_general_reg(0xA, 0x12);
  cpu.set_general_reg(0x1, 0x34);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0xA), static_cast<unsigned char>(0x34 - 0x12));
    EXPECT_EQ(cpu.get_general_reg(0x1), 0x34);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x1);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XY7_overflow) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x83C7;

  cpu.set_general_reg(0x3, 0xAB);
  cpu.set_general_reg(0xC, 0xA1);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0x3), static_cast<unsigned char>(0xA1 - 0xAB));
    EXPECT_EQ(cpu.get_general_reg(0xC), 0xA1);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x0);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XYE_leftmost_zero) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x8A1E;

  cpu.set_general_reg(0x1, 0x14);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0xA), 0x28);
    EXPECT_EQ(cpu.get_general_reg(0x1), 0x14);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x0);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_8XYE_leftmost_one) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x83CE;

  cpu.set_general_reg(0xC, 0xAB);

  try {
    cpu.decode_execute(mem, graphics, keypad);
    EXPECT_EQ(cpu.get_general_reg(0x3), 0x56);
    EXPECT_EQ(cpu.get_general_reg(0xC), 0xAB);
    EXPECT_EQ(cpu.get_general_reg(0xF), 0x1);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_00E0) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0x00E0;

  const std::string seed_str("RNG seed string");
  const std::seed_seq seed(seed_str.begin(), seed_str.end());
  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> pixel_val(0, 1);
  std::uniform_int_distribution<size_t> x_pos(0, arch::graphics::screen_width - 1);
  std::uniform_int_distribution<size_t> y_pos(0, arch::graphics::screen_height - 1);

  for (auto _ = 0; _ < 1000; _++) {
    graphics.set_pixel(x_pos(gen), y_pos(gen), pixel_val(gen));
  }

  cpu.decode_execute(mem, graphics, keypad);

  for (auto i = 0; i < arch::graphics::screen_width; i++) {
    for (auto j = 0; j < arch::graphics::screen_height; j++) {
      EXPECT_EQ(graphics.get_pixel(i, j), 0);
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_DXYN_no_wrap_around_no_collision) {
  std::array<bool, arch::graphics::total_pixels> expected = {};
  // attempt to draw the digit zero.
  // Supposedly looks like this:
  // 11110000 ...
  // 10010000
  // 10010000
  // 10010000
  // 11110000
  // ...
  //
  // The hex equivilant would be
  // 0xF0
  // 0x90
  // 0x90
  // 0x90
  // 0xF0

  expected[0] = 1;
  expected[1] = 1;
  expected[2] = 1;
  expected[3] = 1;
  expected[1 * arch::graphics::screen_width + 0] = 1;
  expected[1 * arch::graphics::screen_width + 3] = 1;
  expected[2 * arch::graphics::screen_width + 0] = 1;
  expected[2 * arch::graphics::screen_width + 3] = 1;
  expected[3 * arch::graphics::screen_width + 0] = 1;
  expected[3 * arch::graphics::screen_width + 3] = 1;
  expected[4 * arch::graphics::screen_width + 0] = 1;
  expected[4 * arch::graphics::screen_width + 1] = 1;
  expected[4 * arch::graphics::screen_width + 2] = 1;
  expected[4 * arch::graphics::screen_width + 3] = 1;

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xD015;
  cpu.set_general_reg(0x0, 0);
  cpu.set_general_reg(0x1, 0);
  cpu.index_reg = 0x300;

  mem.set_value(cpu.index_reg, 0xF0);
  mem.set_value(cpu.index_reg + 1, 0x90);
  mem.set_value(cpu.index_reg + 2, 0x90);
  mem.set_value(cpu.index_reg + 3, 0x90);
  mem.set_value(cpu.index_reg + 4, 0xF0);

  cpu.decode_execute(mem, graphics, keypad);

  for (auto x = 0; x < arch::graphics::screen_width; x++) {
    for (auto y = 0; y < arch::graphics::screen_height; y++) {
      EXPECT_EQ(graphics.get_pixel(x, y), expected[y * arch::graphics::screen_width + x]);
    }
  }

  EXPECT_EQ(cpu.get_general_reg(0xF), 0);
}

TEST(cpu_opcode_test, execute_instruction_DXYN_width_wrap_around_no_collision) {
  std::array<bool, arch::graphics::total_pixels> expected = {};
  // attempt to draw the digit two.
  // Supposedly looks like this:
  // 1 ... 1110000 ...
  // 0 ... 0010000
  // 1 ... 1110000
  // 1 ... 0000000
  // 1 ... 1110000
  // ...
  //
  // The hex equivilant would be
  // 0xF0
  // 0x10
  // 0xF0
  // 0x80
  // 0xF0

  expected[arch::graphics::screen_width - 1] = 1;
  expected[0] = 1;
  expected[1] = 1;
  expected[2] = 1;

  expected[1 * arch::graphics::screen_width + 2] = 1;

  expected[3 * arch::graphics::screen_width - 1] = 1;
  expected[2 * arch::graphics::screen_width + 0] = 1;
  expected[2 * arch::graphics::screen_width + 1] = 1;
  expected[2 * arch::graphics::screen_width + 2] = 1;

  expected[4 * arch::graphics::screen_width - 1] = 1;

  expected[5 * arch::graphics::screen_width - 1] = 1;
  expected[4 * arch::graphics::screen_width + 0] = 1;
  expected[4 * arch::graphics::screen_width + 1] = 1;
  expected[4 * arch::graphics::screen_width + 2] = 1;

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xD345;
  cpu.set_general_reg(0x3, arch::graphics::screen_width - 1);
  cpu.set_general_reg(0x4, 0);
  cpu.index_reg = 0x300;

  mem.set_value(cpu.index_reg, 0xF0);
  mem.set_value(cpu.index_reg + 1, 0x10);
  mem.set_value(cpu.index_reg + 2, 0xF0);
  mem.set_value(cpu.index_reg + 3, 0x80);
  mem.set_value(cpu.index_reg + 4, 0xF0);

  cpu.decode_execute(mem, graphics, keypad);

  for (auto x = 0; x < arch::graphics::screen_width; x++) {
    for (auto y = 0; y < arch::graphics::screen_height; y++) {
      EXPECT_EQ(graphics.get_pixel(x, y), expected[y * arch::graphics::screen_width + x]);
    }
  }

  EXPECT_EQ(cpu.get_general_reg(0xF), 0);
}

TEST(cpu_opcode_test, execute_instruction_DXYN_height_wrap_around_no_collision) {
  std::array<bool, arch::graphics::total_pixels> expected = {};
  // attempt to draw the digit seven.
  // Supposedly looks like this:
  // 11110000
  // 00010000
  // 00100000
  // 01000000
  // 01000000
  //
  // The hex equivilant would be
  // 0xF0
  // 0x10
  // 0x20
  // 0x40
  // 0x40

  expected[(arch::graphics::screen_height - 1) * arch::graphics::screen_width + 1] = 1;
  expected[(arch::graphics::screen_height - 1) * arch::graphics::screen_width + 2] = 1;
  expected[(arch::graphics::screen_height - 1) * arch::graphics::screen_width + 3] = 1;
  expected[(arch::graphics::screen_height - 1) * arch::graphics::screen_width + 4] = 1;

  // wrap around part
  expected[4] = 1;
  expected[arch::graphics::screen_width + 3] = 1;
  expected[2 * arch::graphics::screen_width + 2] = 1;
  expected[3 * arch::graphics::screen_width + 2] = 1;

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xDAB5;
  cpu.set_general_reg(0xA, 1);
  cpu.set_general_reg(0xB, arch::graphics::screen_height - 1);
  cpu.index_reg = 0x300;

  mem.set_value(cpu.index_reg, 0xF0);
  mem.set_value(cpu.index_reg + 1, 0x10);
  mem.set_value(cpu.index_reg + 2, 0x20);
  mem.set_value(cpu.index_reg + 3, 0x40);
  mem.set_value(cpu.index_reg + 4, 0x40);

  cpu.decode_execute(mem, graphics, keypad);

  for (auto x = 0; x < arch::graphics::screen_width; x++) {
    for (auto y = 0; y < arch::graphics::screen_height; y++) {
      EXPECT_EQ(graphics.get_pixel(x, y), expected[y * arch::graphics::screen_width + x]);
    }
  }

  EXPECT_EQ(cpu.get_general_reg(0xF), 0);
}

TEST(cpu_opcode_test, execute_instruction_DXYN_no_wrap_around_collision) {
  std::array<bool, arch::graphics::total_pixels> expected = {};

  expected[1] = 1;
  expected[2] = 1;
  expected[5] = 1;
  expected[6] = 1;

  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xD561;
  cpu.set_general_reg(0x5, 3);
  cpu.set_general_reg(0x6, 0);
  cpu.index_reg = 0x300;

  mem.set_value(cpu.index_reg, 0xF0);

  graphics.set_pixel(1, 0, true);
  graphics.set_pixel(2, 0, true);
  graphics.set_pixel(3, 0, true);
  graphics.set_pixel(4, 0, true);

  cpu.decode_execute(mem, graphics, keypad);

  for (auto x = 0; x < arch::graphics::screen_width; x++) {
    for (auto y = 0; y < arch::graphics::screen_height; y++) {
      EXPECT_EQ(graphics.get_pixel(x, y), expected[y * arch::graphics::screen_width + x]);
    }
  }

  EXPECT_EQ(cpu.get_general_reg(0xF), 1);
}

TEST(cpu_opcode_test, execute_instruction_FX1E) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.index_reg = 0x100;

  cpu.curr_opcode = 0xF11E;
  cpu.set_general_reg(0x1, 0xAB);
  cpu.decode_execute(mem, graphics, keypad);
  EXPECT_EQ(cpu.index_reg, 0x100 + 0xAB);

  cpu.curr_opcode = 0xFA1E;
  cpu.set_general_reg(0xA, 0x42);
  cpu.decode_execute(mem, graphics, keypad);
  EXPECT_EQ(cpu.index_reg, 0x100 + 0xAB + 0x42);

  cpu.curr_opcode = 0xF51E;
  cpu.set_general_reg(0x5, 0x1);
  cpu.decode_execute(mem, graphics, keypad);
  EXPECT_EQ(cpu.index_reg, 0x100 + 0xAB + 0x42 + 0x1);
}

TEST(cpu_opcode_test, execute_instruction_FX29) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xFF29;
  cpu.set_general_reg(0xF, 0x10);
  cpu.decode_execute(mem, graphics, keypad);
  EXPECT_EQ(cpu.index_reg, 0);

  cpu.curr_opcode = 0xFA29;
  cpu.set_general_reg(0xA, 0x03);
  cpu.decode_execute(mem, graphics, keypad);
  EXPECT_EQ(cpu.index_reg, 15);

  cpu.curr_opcode = 0xF329;
  cpu.set_general_reg(0x3, 0x8);
  cpu.decode_execute(mem, graphics, keypad);
  EXPECT_EQ(cpu.index_reg, 40);
}

TEST(cpu_opcode_test, execute_instruction_FX33_ones) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xF133;
  cpu.set_general_reg(0x1, 5);
  cpu.index_reg = 0x300;

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(mem.get_value(cpu.index_reg), 0);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 1), 0);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 2), 5);
}

TEST(cpu_opcode_test, execute_instruction_FX33_tens) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xF633;
  cpu.set_general_reg(0x6, 30);
  cpu.index_reg = 0x300;

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(mem.get_value(cpu.index_reg), 0);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 1), 3);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 2), 0);
}

TEST(cpu_opcode_test, execute_instruction_FX33_hundreds) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xFA33;
  cpu.set_general_reg(0xA, 200);
  cpu.index_reg = 0x300;

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(mem.get_value(cpu.index_reg), 2);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 1), 0);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 2), 0);
}

TEST(cpu_opcode_test, execute_instruction_FX33_generic_num) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xF233;
  cpu.set_general_reg(0x2, 125);
  cpu.index_reg = 0x300;

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(mem.get_value(cpu.index_reg), 1);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 1), 2);
  EXPECT_EQ(mem.get_value(cpu.index_reg + 2), 5);
}

TEST(cpu_opcode_test, execute_instruction_FX55) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xF355;
  cpu.set_general_reg(0x0, 125);
  cpu.set_general_reg(0x1, 35);
  cpu.set_general_reg(0x2, 70);
  cpu.set_general_reg(0x3, 23);
  cpu.index_reg = 0x300;

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(mem.get_value(0x300), 125);
  EXPECT_EQ(mem.get_value(0x300 + 1), 35);
  EXPECT_EQ(mem.get_value(0x300 + 2), 70);
  EXPECT_EQ(mem.get_value(0x300 + 3), 23);

  EXPECT_EQ(cpu.index_reg, 0x300 + 0x3 + 1);
}

TEST(cpu_opcode_test, execute_instruction_FX65) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xF565;
  cpu.index_reg = 0x300;

  mem.set_value(cpu.index_reg, 0x34);
  mem.set_value(cpu.index_reg + 1, 0xF3);
  mem.set_value(cpu.index_reg + 2, 0x3C);
  mem.set_value(cpu.index_reg + 3, 0x1A);
  mem.set_value(cpu.index_reg + 4, 0xAF);
  mem.set_value(cpu.index_reg + 5, 0x79);

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(cpu.get_general_reg(0x0), 0x34);
  EXPECT_EQ(cpu.get_general_reg(0x1), 0xF3);
  EXPECT_EQ(cpu.get_general_reg(0x2), 0x3C);
  EXPECT_EQ(cpu.get_general_reg(0x3), 0x1A);
  EXPECT_EQ(cpu.get_general_reg(0x4), 0xAF);
  EXPECT_EQ(cpu.get_general_reg(0x5), 0x79);

  EXPECT_EQ(cpu.index_reg, 0x300 + 0x5 + 1);
}

TEST(cpu_opcode_test, execute_instruction_EX9E_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xE19E;
  cpu.set_general_reg(0x1, 2);
  cpu.pc_reg = 0x200;

  keypad.press_key(2);

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(cpu.pc_reg, 0x200 + 2);
}

TEST(cpu_opcode_test, execute_instruction_EX9E_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xE49E;
  cpu.set_general_reg(0x4, 5);
  cpu.pc_reg = 0x200;

  keypad.release_key(5);

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(cpu.pc_reg, 0x200);
}

TEST(cpu_opcode_test, execute_instruction_EXA1_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xE1A1;
  cpu.set_general_reg(0x1, 7);
  cpu.pc_reg = 0x200;

  keypad.release_key(7);

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(cpu.pc_reg, 0x200 + 2);
}

TEST(cpu_opcode_test, execute_instruction_EXA1_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};
  arch::Graphics graphics{};
  arch::Keypad keypad{};

  cpu.curr_opcode = 0xE0A1;
  cpu.set_general_reg(0x0, 9);
  cpu.pc_reg = 0x200;

  keypad.press_key(9);

  cpu.decode_execute(mem, graphics, keypad);

  EXPECT_EQ(cpu.pc_reg, 0x200);
}