#pragma once
#ifndef CHIP8EMULATOR_ARCH_CPU_H_
#  define CHIP8EMULATOR_ARCH_CPU_H_

#  include <array>
#  include <stdexcept>

#  include "memory.h"

namespace arch {
  constexpr size_t num_general_reg = 16;  // Number of general purpose registers
  constexpr size_t stack_size = 16;       // Depth of nested subroutine calls

  class CPU {
  public:
    CPU();

    void fetch(Memory& mem);

    void decode_execute(Memory& mem);

    // Current opcode. Meant to be set via fetch and used via decode_execute
    unsigned short curr_opcode;

    unsigned char get_general_reg(size_t reg_id) const;

    unsigned short get_index_reg() const;

    unsigned short get_pc_reg() const;

    unsigned short get_sp_reg() const;

  private:
    // Registers
    std::array<unsigned char, num_general_reg> general_reg;  // General purpose registers 16 8 bit
    unsigned short index_reg;                                // Index register 16 bit
    unsigned short pc_reg;                                   // Program counter register 16 bit
    unsigned short sp_reg;                                   // Stack pointer register 16 bit
    unsigned char delay_timer_reg;                           // Delay timer register 8 bits
    unsigned char sound_timer_reg;                           // Sound timer register 8 bits

    // Stack for storing return addresses
    std::array<unsigned short, stack_size> stack;  // Meant to store return addresses
  };

  class InvalidRegisterID : public std::exception {
  public:
    virtual const char* what() const throw() {
      return "Invalid general register ID given. IDs span from 0x0 to 0xE inclusive.\n";
    }
  };

  class InvalidOpCode : public std::exception {
  public:
    virtual const char* what() const throw() { return "Invalid opcode given\n";
    }
  };
}  // namespace arch

#endif  // !CHIP8EMULATOR_ARCH_CPU_H_
