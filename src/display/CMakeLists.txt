# ==================================================================================================
# Library to handle screen rendering
# ==================================================================================================

set(DISPLAY_HEADERS "display.h" "input_events.h")
set(DISPLAY_SOURCES "display.cpp")

find_package(SDL2 REQUIRED)
add_library(Display ${DISPLAY_HEADERS} ${DISPLAY_SOURCES})

target_include_directories(Display PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Display PRIVATE SDL2::SDL2)
target_compile_features(Display PRIVATE cxx_std_20)