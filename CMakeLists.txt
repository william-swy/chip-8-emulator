# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("chip8_emulator" VERSION 1.0.0 LANGUAGES CXX)

# Include sub-projects.
add_subdirectory ("src")
