#include <gtest/gtest.h>

#include <array>

#include "arch/cpu.h"
#include "arch/memory.h"

TEST(cpu_opcode_test, execute_instruction_ANNN_once) {
  arch::CPU cpu{};
  arch::Memory mem{};

  cpu.curr_opcode = 0xA123;

  try {
    cpu.decode_execute(mem);
    EXPECT_EQ(cpu.index_reg, 0x0123);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_ANNN_many_times) {
  arch::CPU cpu{};
  arch::Memory mem{};

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
      cpu.decode_execute(mem);
      EXPECT_EQ(cpu.index_reg, val.expected);
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_BNNN_once) {
  arch::CPU cpu;
  arch::Memory mem;

  cpu.set_general_reg(0, 0xF1);
  cpu.curr_opcode = 0xB234;

  try {
    cpu.decode_execute(mem);
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

  for (const auto val : input) {
    cpu.set_general_reg(0, val.reg_v0);
    cpu.curr_opcode = val.opcode;

    try {
      cpu.decode_execute(mem);
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

  cpu.curr_opcode = 0xB123;

  try {
    cpu.decode_execute(mem);
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

  arch::CPU cpu;
  arch::Memory mem;

  for (const auto& val : input) {
    try {
      cpu.curr_opcode = val.opcode;
      cpu.decode_execute(mem);
      EXPECT_EQ(cpu.pc_reg, val.expected_address);
    } catch (const arch::InvalidInstruction&) {
      FAIL() << "InvalidInstruction exception should not have been thrown\n";
    }
  }
}

TEST(cpu_opcode_test, execute_instruction_3XNN_once_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x0, 0x11);
  cpu.curr_opcode = 0x3012;

  try {
    cpu.decode_execute(mem);
    EXPECT_EQ(cpu.pc_reg, 0x3460);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_3XNN_once_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x4, 0x25);
  cpu.curr_opcode = 0x3425;

  try {
    cpu.decode_execute(mem);
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

  for (const auto& val : input_vals) {
    cpu.pc_reg = val.start_pc;
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.register_id, val.register_value);
    try {
      cpu.decode_execute(mem);
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

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x0, 0x12);
  cpu.curr_opcode = 0x4012;

  try {
    cpu.decode_execute(mem);
    EXPECT_EQ(cpu.pc_reg, 0x3460);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_4XNN_once_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};

  cpu.pc_reg = 0x3460;
  cpu.set_general_reg(0x4, 0xAF);
  cpu.curr_opcode = 0x4425;

  try {
    cpu.decode_execute(mem);
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

  for (const auto& val : input_vals) {
    cpu.pc_reg = val.start_pc;
    cpu.curr_opcode = val.opcode;
    cpu.set_general_reg(val.register_id, val.register_value);
    try {
      cpu.decode_execute(mem);
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

  cpu.curr_opcode = 0x53F1;

  try {
    cpu.decode_execute(mem);
    FAIL() << "InvalidInstruction exception should have been thrown.\n";
  } catch (const arch::InvalidInstruction&) {
    SUCCEED();
  }
}

TEST(cpu_opcode_test, execute_instruction_5XY0_once_no_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};

  cpu.curr_opcode = 0x57F0;
  cpu.set_general_reg(0x7, 0x3);
  cpu.set_general_reg(0xF, 0x2);
  cpu.pc_reg = 0x300;

  try {
    cpu.decode_execute(mem);
    EXPECT_EQ(cpu.pc_reg, 0x300);
  } catch (const arch::InvalidInstruction&) {
    FAIL() << "InvalidInstruction exception should not have been thrown.\n";
  }
}

TEST(cpu_opcode_test, execute_instruction_5XY0_once_skip) {
  arch::CPU cpu{};
  arch::Memory mem{};

  cpu.curr_opcode = 0x54B0;
  cpu.set_general_reg(0x4, 0xF);
  cpu.set_general_reg(0xB, 0xF);
  cpu.pc_reg = 0x512;

  try {
    cpu.decode_execute(mem);
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

  for (const auto& val : inputs) {
    cpu.curr_opcode = val.opcode;
    cpu.pc_reg = val.curr_pc;
    cpu.set_general_reg(val.reg_X, val.reg_X_val);
    cpu.set_general_reg(val.reg_Y, val.reg_Y_val);
    try {
      cpu.decode_execute(mem);
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