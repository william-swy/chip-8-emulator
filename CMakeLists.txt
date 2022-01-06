cmake_minimum_required(VERSION 3.15)

project(
  "chip8_emulator"
  VERSION 0.2.0
  DESCRIPTION "CHIP-8 Emulator"
  LANGUAGES CXX
)

# Prohibit in source builds
include("cmake/prevent_insource.cmake")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE
      RelWithDebInfo
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS
             "Debug"
             "Release"
             "MinSizeRel"
             "RelWithDebInfo")
endif()

# "library" to add compile options
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

# "library" to add compile warnings
include("cmake/compiler_warnings.cmake")
add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)

# Sanitizers
include("cmake/sanitizers.cmake")
enable_sanitizers(project_options)

# Static analyzers
include("cmake/static_analyzers.cmake")

# Link time optimization
option(ENABLE_IPO "Link Time Optimization" OFF)

if(ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(
    RESULT
    result
    OUTPUT
    output)
  if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
  else()
    message(SEND_ERROR "IPO is not supported: ${output}")
  endif()
endif()

# Recommended to use Conan for dependency management
option(ENABLE_CONAN "Use Conan for dependency management" ON)
if(ENABLE_CONAN)
  include("cmake/Conan.cmake")
  run_conan()
endif()

# Testing
option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests")
  add_subdirectory("test")
endif()


# Source code - DUH
add_subdirectory("src")



