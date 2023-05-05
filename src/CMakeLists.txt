# ==================================================================================================
# Executable for emulation
# ==================================================================================================

add_subdirectory("arch")

add_subdirectory("display")

add_subdirectory("sdl_interface")

add_executable(chip8_emulator "main.cpp" "chip8.cpp" "chip8.h" "display/input_events.h")

target_link_libraries(chip8_emulator PRIVATE Display Arch sdl_interface project_options project_warnings)
