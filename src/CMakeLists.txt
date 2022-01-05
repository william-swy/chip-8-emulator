# ===================================================================================================
# Executable for emulation
# ===================================================================================================
# add_executable(chip8_emulator "chip8_emulator.cpp" "chip8_emulator.h" "chip8.h")

add_subdirectory("arch")

add_subdirectory("display")

add_executable(chip8_emulator "main.cpp" "chip8.cpp" "chip8.h"  "display/input_events.h")

target_link_libraries(chip8_emulator PRIVATE Display Arch project_options project_warnings)
