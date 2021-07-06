# ==================================================================================================
# Library to handle emulation logic
# ==================================================================================================

set(ARCH_HEADERS "cpu.h" "graphics.h" "keypad.h" "memory.h")
set(ARCH_SOURCES "cpu.cpp" "memory.cpp" "graphics.cpp" "keypad.cpp")

add_library(Arch ${ARCH_HEADERS} ${ARCH_SOURCES})
target_include_directories(Arch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Arch PUBLIC cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(Arch PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
  target_compile_options(Arch PUBLIC /external:anglebrackets /external:W0 /Wall /W3 /wd5045)
endif()