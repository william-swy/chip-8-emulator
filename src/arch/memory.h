#pragma once

#include <array>
#include <stdexcept>
#include <string>

namespace arch {
  constexpr std::size_t mem_size{4096};                 // Total RAM size in bytes
  constexpr std::size_t max_mem_address{mem_size - 1};  // Max address value

  class Memory {
  public:
    Memory() = default;

    [[nodiscard]] unsigned char get_value(unsigned short address) const;

    void set_value(unsigned short address, unsigned char value);

  private:
    // Each index holds one byte of data for a total of 4KB RAM size
    std::array<unsigned char, mem_size> mem{};
  };

  class InvalidMemoryAddress : public std::exception {
    std::string what_msg{"Invalid address given. Must be between 0 and "
                         + std::to_string(max_mem_address)};

  public:
    virtual const char* what() const noexcept { return what_msg.c_str(); }
  };
}  // namespace arch
