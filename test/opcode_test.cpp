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

TEST(cpu_opcode_test, execute_instruction_3XNN_many_assorted_jumps) {
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