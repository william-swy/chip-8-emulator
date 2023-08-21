find_package(SDL2 REQUIRED)

add_library(sdl_interface STATIC "sound.cpp" "sound.h")
target_include_directories(sdl_interface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sdl_interface PRIVATE SDL2::SDL2 project_options project_warnings)
