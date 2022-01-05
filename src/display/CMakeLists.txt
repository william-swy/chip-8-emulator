# ==================================================================================================
# Library to handle screen rendering
# ==================================================================================================

find_package(SDL2 REQUIRED)
add_library(Display STATIC "display.h" "input_events.h" "display.cpp")

target_include_directories(Display PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Display PRIVATE SDL2::SDL2 project_options project_warnings)
