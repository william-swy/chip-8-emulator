# ===================================================================================================
# Executable for emulation
# ===================================================================================================
# add_executable(chip8_emulator "chip8_emulator.cpp" "chip8_emulator.h" "chip8.h")

add_subdirectory("arch")

add_subdirectory("display")

add_executable(chip8_emulator "main.cpp" "chip8.cpp" "chip8.h"  "display/input_events.h")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(chip8_emulator PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
  target_compile_options(
    chip8_emulator PUBLIC /Wall /W3 /external:anglebrackets /external:W0 /wd5045
  )
endif()

target_link_libraries(chip8_emulator PRIVATE Display Arch)
