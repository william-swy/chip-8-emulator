# CMakeList.txt : Top-level CMake project file, do global configuration and include sub-projects
# here.
#
cmake_minimum_required(VERSION 3.10)

project(
  "chip8_emulator"
  VERSION 0.1.0
  LANGUAGES CXX
)

# Prohibit in source builds
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# Include sub-projects.
add_subdirectory("src")

add_subdirectory("test")
