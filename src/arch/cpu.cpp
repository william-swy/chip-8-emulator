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
}

void arch::CPU::decode_execute(Memory& mem) {
  // Parse out first 4 bits
  switch (curr_opcode & 0xF000) {
    case 0x0000:
      break;
    case 0x1000:
      break;
    case 0x2000:
      break;
    case 0x3000:
      break;
    case 0x4000:
      break;
    case 0x5000:
      break;
    case 0x6000:
      break;
    case 0x7000:
      break;
    case 0x8000:
      break;
    case 0x9000:
      break;
    case 0xA000:
      // Of form ANNN. Stores memory address NNN in index register

      break;
    case 0xB000:
      break;
    case 0xC000:
      break;
    case 0xD000:
      break;
    case 0xE000:
      break;
    case 0xF000:
      break;
    default:
        // This is actually technically impossible to reach
      throw InvalidOpCode();
  }
}

unsigned char arch::CPU::get_general_reg(size_t reg_id) const {
  if (reg_id >= num_general_reg) {
    throw InvalidRegisterID();
  } else {
    return general_reg[reg_id];
  }
}
