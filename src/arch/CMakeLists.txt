# ==================================================================================================
# Library to handle emulation logic
# ==================================================================================================

set(ARCH_HEADERS "cpu.h" "graphics.h" "keypad.h" "memory.h" "timer.h")
set(ARCH_SOURCES "cpu.cpp" "memory.cpp" "graphics.cpp" "keypad.cpp" "timer.cpp")

add_library(Arch STATIC ${ARCH_HEADERS} ${ARCH_SOURCES})
target_include_directories(Arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Arch PRIVATE project_options project_warnings)
