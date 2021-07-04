# CMakeList.txt : CMake project for chip8_emulator, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Add source to this project's executable.
add_executable (chip8_emulator "chip8_emulator.cpp" "chip8_emulator.h" "arch/graphics.h" "arch/keypad.h" "chip8.h" "arch/cpu.cpp" "arch/memory.cpp" "arch/graphics.cpp")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(chip8_emulator PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
	target_compile_options(chip8_emulator PUBLIC /Wall /W4 /WX /external:anglebrackets /external:W0)
endif()

target_compile_features(chip8_emulator PUBLIC cxx_std_20)

set(ARCH_HEADERS "arch/cpu.h" "arch/graphics.h" "arch/keypad.h" "arch/memory.h")
set(ARCH_SOURCES "arch/cpu.cpp" "arch/memory.cpp" "arch/graphics.cpp" "arch/keypad.cpp")

add_library(Arch ${ARCH_HEADERS} ${ARCH_SOURCES})
target_include_directories(Arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Arch PUBLIC cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(
		Arch 
		PUBLIC 
		-Wall 
		-Wpedantic 
		-Wextra 
		-Werror)
elseif(MSVC)
	target_compile_options(
		Arch 
		PUBLIC 
		/external:anglebrackets 
		/external:W0 
		/Wall 
		/W3 
		/wd5045
	)
endif()