#pragma once
#ifndef CHIP8EMULATOR_ARCH_CPU_H_
#  define CHIP8EMULATOR_ARCH_CPU_H_

#  include <array>
#  include <random>
#  include <stdexcept>
#  include <string>

#  include "graphics.h"
#  include "keypad.h"
#  include "memory.h"

namespace arch {
  constexpr size_t num_general_reg = 16;            // Number of general purpose registers
  constexpr size_t stack_size = 16;                 // Depth of nested subroutine calls
  constexpr unsigned short pc_start_value = 0x200;  // Initial value of PC when booted

  class CPU {
  public:
    CPU();

    void fetch(Memory& mem);

    void decode_execute(Memory& mem, Graphics& graphics, Keypad& keypad);

    // Getters and setters for general registers and stack to make sure that only valid indices are
    // provided. Although they are both std::array which has its own bounds checking, by doing the
    // checking through getter and setter, there is control over the type of exception thrown which
    // allows it to be more description of what the exception is instead of an out of bounds for
    // both scenarios.
    [[nodiscard]] unsigned char get_general_reg(size_t reg_idx) const;

    void set_general_reg(size_t reg_idx, unsigned char value);

    [[nodiscard]] unsigned short get_stack_pointer() const;

    void set_stack_pointer(unsigned char value);

    [[nodiscard]] unsigned short get_stack() const;

    void set_stack(unsigned short value);

    // Current opcode. Meant to be set via fetch and used via decode_execute
    unsigned short curr_opcode;

    unsigned short index_reg;       // Index register 16 bit
    unsigned short pc_reg;          // Program counter register 16 bit
    unsigned char delay_timer_reg;  // Delay timer register 8 bits
    unsigned char sound_timer_reg;  // Sound timer register 8 bits

    bool updated_screen;

  private:
    // Registers
    std::array<unsigned char, num_general_reg> general_reg;  // General purpose registers 16 8 bit

    unsigned char sp_reg;  // Stack pointer register 16 bit
    // Stack for storing return addresses
    std::array<unsigned short, stack_size + 1> stack;  // Meant to store return addresses

    // RNG
    std::mt19937 gen;
    std::uniform_int_distribution<> rng;
  };

  class InvalidRegisterID : public std::exception {
  public:
    virtual const char* what() const noexcept {
      return "Invalid general register ID given. IDs span from 0x0 to 0xF inclusive.\n";
    }
  };

  class InvalidInstruction : public std::exception {
  public:
    virtual const char* what() const noexcept { return "Invalid instruction given\n"; }
  };

  class InvalidStackPointerValue : public std::exception {
  public:
    virtual const char* what() const noexcept {
      return "Invalid value for stack pointer. Cannot be equal to or exceed stack size.\n";
    }
  };
}  // namespace arch

#endif  // !CHIP8EMULATOR_ARCH_CPU_H_
