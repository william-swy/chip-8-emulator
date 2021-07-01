#include "cpu.h"

#include "memory.h"

arch::CPU::CPU() {
  index_reg = 0;
  pc_reg = pc_start_value;
  sp_reg = 0;

  general_reg = {};
  delay_timer_reg = 0;
  sound_timer_reg = 0;
  stack = {};
  curr_opcode = 0;
}

void arch::CPU::fetch(Memory& mem) {
  // Instructions are 2 bytes, hence need to get current program counter and the next address to
  // build opcode. It is also big endian, hence mem[PC] represents the left most value and mem[PC+1]
  // represents the right most value. Will throw a InvalidMemoryAddress exception if PC holds a
  // value that is either at the greatest memory address or greater.

  const auto left_most = static_cast<unsigned>(mem.get_value(pc_reg));
  const auto right_most = static_cast<unsigned>(mem.get_value(pc_reg + 1));
  curr_opcode = static_cast<unsigned short>(left_most << 8 | right_most);
  pc_reg += 2;
}

void arch::CPU::decode_execute(Memory&) {
  // Parse out first 4 bits
  switch (curr_opcode & 0xF000) {
    case 0x0000:
      // TODO
      break;
    case 0x1000:
      // Of form 1NNN. Jumps to address NNN
      {
        const auto address = static_cast<unsigned short>(curr_opcode & 0x0FFF);
        pc_reg = address;
      }
      break;
    case 0x2000:
      // TODO
      break;
    case 0x3000:
      // Of form 3XNN. Skips the next instruction if value of register X equals NN
      {
        // Mask to get the register id and bitshift right 8 to remove the two trailing zeros.
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);
        if (general_reg[reg_id] == value) {
          pc_reg += 2;
        }
      }
      break;
    case 0x4000:
      // Of form 4XNN. Skips the next instruction if the value of register X does not equal NN
      {
        // Mask to get the register id and bitshift right 8 to remove the two trailing zeros.
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);
        if (general_reg[reg_id] != value) {
          pc_reg += 2;
        }
      }
      break;
    case 0x5000:
      // Must be of form 5XY0. Skips the next instruction of the value of register X equals the
      // value of register Y. The right most value must be a 0.
      if ((curr_opcode & 0x00F) != 0) {
        throw InvalidInstruction();
      } else {
        const auto reg_X = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto reg_Y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);
        if (general_reg[reg_X] == general_reg[reg_Y]) {
          pc_reg += 2;
        }
      }
      break;
    case 0x6000:
      // Of form 6XNN. Stores the value NN in the register X
      {
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);

        general_reg[reg_id] = value;
      }
      break;
    case 0x7000:
      // Of form 7XNN. Adds the value of NN in the register X
      {
        const auto reg_id = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto value = static_cast<unsigned char>(curr_opcode & 0x00FF);
        general_reg[reg_id] += value;
      }
      break;
    case 0x8000:
      // TODO
      break;
    case 0x9000:
      // Must be of form 9XY0. Skips the next instruction if the value of register X does not equal
      // the value of register Y. The right most value must be a 0.
      if ((curr_opcode & 0x00F) != 0) {
        throw InvalidInstruction();
      } else {
        const auto reg_X = static_cast<size_t>((curr_opcode & 0x0F00) >> 8);
        const auto reg_Y = static_cast<size_t>((curr_opcode & 0x00F0) >> 4);
        if (general_reg[reg_X] != general_reg[reg_Y]) {
          pc_reg += 2;
        }
      }
      break;
    case 0xA000:
      // Of form ANNN. Stores memory address NNN in index register
      index_reg = static_cast<unsigned short>(curr_opcode & 0x0FFF);
      break;
    case 0xB000:
      // Of form BNNN. Jump to address NNN + value in register 0
      {
        const auto jump_offset = static_cast<unsigned short>(curr_opcode & 0x0FFF);
        pc_reg = static_cast<unsigned short>(jump_offset + general_reg[0]);
      }
      break;
    case 0xC000:
      // TODO
      break;
    case 0xD000:
      // TODO
      break;
    case 0xE000:
      // TODO
      break;
    case 0xF000:
      // TODO
      break;
    default:
      // This is actually technically impossible to reach
      throw InvalidInstruction();
  }
}

unsigned char arch::CPU::get_general_reg(size_t reg_id) const {
  if (reg_id >= num_general_reg) {
    throw InvalidRegisterID();
  } else {
    return general_reg[reg_id];
  }
}

void arch::CPU::set_general_reg(size_t reg_idx, unsigned char value) {
  if (reg_idx >= num_general_reg) {
    throw InvalidRegisterID();
  } else {
    general_reg[reg_idx] = value;
  }
}

unsigned short arch::CPU::get_stack_pointer() const { return sp_reg; }

void arch::CPU::set_stack_pointer(unsigned short value) {
  if (value >= stack_size) {
    throw InvalidStackPointerValue();
  } else {
    sp_reg = value;
  }
}

unsigned short arch::CPU::get_stack() const { return stack[sp_reg]; }

void arch::CPU::set_stack(unsigned short value) { stack[sp_reg] = value; }
