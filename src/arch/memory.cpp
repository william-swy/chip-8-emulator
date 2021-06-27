#include "memory.h"

unsigned char arch::Memory::get_value(unsigned short address) const {
  if (address > max_mem_address) {
    throw InvalidMemoryAddress();
  } else {
    return mem[address];
  }
}

void arch::Memory::set_value(unsigned short address, unsigned char value) {
  if (address > max_mem_address) {
    throw InvalidMemoryAddress();
  } else {
    mem[address] = value;
  }
}