cmake_minimum_required(VERSION 3.15)

project(
  "chip8_emulator"
  VERSION 0.2.0
  DESCRIPTION "CHIP-8 Emulator"
  LANGUAGES CXX
)

# Prohibit in source builds
include("cmake/prevent_insource.cmake")

# Build types
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE
      RelWithDebInfo
      CACHE STRING "Choose the type of build." FORCE
  )

  set_property(
    CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
  )
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
  check_ipo_supported(RESULT ipo_result OUTPUT ipo_output)
  if(ipo_result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
  else()
    message(SEND_ERROR "IPO is not supported: ${ipo_output}")
  endif()
endif()

if(NOT PACKAGE_MANAGER)
  message(STATUS "Setting PACKAGE_MANAGER to 'None' as none was specified")
  set(PACKAGE_MANAGER
      None
      CACHE STRING "Choose the package manager." FORCE
  )

  set_property(CACHE PACKAGE_MANAGER PROPERTY STRINGS "Conan" "vcpkg" "None")
endif()

message(STATUS "Using package manager: ${PACKAGE_MANAGER}")

if(PACKAGE_MANAGER STREQUAL "Conan")
  include("cmake/Conan.cmake")
  run_conan()
elseif(PACKAGE_MANAGER STREQUAL "vcpkg")
  if(NOT CMAKE_TOOL_CHAIN_FILE)
    message(FATAL_ERROR " CMAKE_TOOL_CHAIN_FILE not specified. "
                        " Provide the value of <vcpkg_install_dir/scripts/buildsystems/vcpkg.cmake>"
    )
  endif()
  if(NOT CMAKE_TOOL_CHAIN_FILE MATCHES ".*/scripts/buildsystems/vcpkg.cmake")
    message(FATAL_ERROR " CMAKE_TOOL_CHAIN_FILE value invalid. "
                        " Provide the value of <vcpkg_install_dir/scripts/buildsystems/vcpkg.cmake>"
    )
  endif()
endif()

option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  enable_testing()
  message(STATUS "Building Tests")
  add_subdirectory("test")
endif()

add_subdirectory("src")
