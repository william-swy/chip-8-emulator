#include "cpu.h"

#include "memory.h"

arch::CPU::CPU() {
  index_reg = 0;
  pc_reg = 0x200;
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
  // represents the right most value.

  unsigned char left_most = mem.get_value(pc_reg);
  unsigned char right_most = mem.get_value(pc_reg + 1);
  curr_opcode = left_most << 8 | right_most;
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
      break;
  }
}