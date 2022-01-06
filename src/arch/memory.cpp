#include "memory.h"

unsigned char arch::Memory::get_value(unsigned short address) const {
  try {
    return mem.at(address);
  } catch (const std::out_of_range&) {
    throw InvalidMemoryAddress();
  }
}

void arch::Memory::set_value(unsigned short address, unsigned char value) {
  try {
    mem.at(address) = value;
  } catch (const std::out_of_range&) {
    throw InvalidMemoryAddress();
  }
}