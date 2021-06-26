#pragma once
#ifndef CHIP8EMULATOR_ARCH_MEMORY_H_
#  define CHIP8EMULATOR_ARCH_MEMORY_H_

#  include <array>

namespace arch {
  constexpr size_t mem_size = 4096;  // Total RAM size in bytes

  class Memory {
  public:
    Memory() = default;

    [[nodiscard]] unsigned char get_value(unsigned short address) const;

    void set_value(unsigned short address, unsigned char value);

  private:
    // Each index holds one byte of data for a total of 4KB RAM size
    std::array<unsigned char, mem_size> mem;
  };
}  // namespace arch

#endif  // !CHIP8EMULATOR_ARCH_MEMORY_H_
