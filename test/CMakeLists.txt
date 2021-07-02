cmake_minimum_required (VERSION 3.10)

project(chip8_emulator_tests LANGUAGES CXX)

enable_testing()

find_package(GTest REQUIRED)
include(GoogleTest)

set(TEST_SOURCES "memory_test.cpp" "cpu_test.cpp" "opcode_test.cpp" "graphics_test.cpp")

add_executable(
	chip8_emulator_tests
	${TEST_SOURCES}
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
	Arch
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(chip8_emulator_tests PUBLIC -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
	# Get \Qspectre warnings to be silenced
	target_compile_options(
		chip8_emulator_tests
		PUBLIC
		/external:anglebrackets 
		/external:W0 
		/external:templates- 
		/Wall 
		/W4
		/wd5045
	)
endif()

target_compile_features(chip8_emulator_tests PUBLIC cxx_std_20)


gtest_discover_tests(chip8_emulator_tests)