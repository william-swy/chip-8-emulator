# CMakeList.txt : CMake project for chip8_emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Add source to this project's executable.
add_executable (chip8_emulator "chip8_emulator.cpp" "chip8_emulator.h" "arch/graphics.h" "arch/keypad.h" "chip8.h" "arch/cpu.cpp" "arch/memory.cpp")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(chip8_emulator PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
	target_compile_options(chip8_emulator PUBLIC /Wall /W4 /WX)
endif()

target_compile_features(chip8_emulator PUBLIC cxx_std_20)
