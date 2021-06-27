cmake_minimum_required (VERSION 3.10)

project(chip8_emulator_tests LANGUAGES CXX)

enable_testing()

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(
	chip8_emulator_tests
	memory_test.cpp
)

target_include_directories(
	chip8_emulator_tests
	PRIVATE
	${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
	chip8_emulator_tests
	PRIVATE
	GTest::gtest
	GTest::gtest_main
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(chip8_emulator_tests PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
	target_compile_options(chip8_emulator_tests PUBLIC /Wall /W4)
endif()


gtest_discover_tests(chip8_emulator_tests)