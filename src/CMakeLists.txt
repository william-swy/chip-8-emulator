# CMakeList.txt : CMake project for chip8_emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (chip8_emulator "chip8_emulator.cpp" "chip8_emulator.h" "arch/graphics.h" "arch/keypad.h" "chip8.h" "arch/cpu.cpp")

target_compile_options(chip8_emulator PRIVATE -Wpedantic -Wextra -Werror)
target_compile_features(chip8_emulator PRIVATE cxx_std_20)


# TODO: Add tests and install targets if needed.
