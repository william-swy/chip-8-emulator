cmake_minimum_required (VERSION 3.10)

project(chip8_emulator_tests LANGUAGES CXX)

enable_testing()

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(
	chip8_emulator_tests
	memory_test.cpp
	"${CMAKE_SOURCE_DIR}/src/arch/memory.cpp"
)

target_include_directories(
	chip8_emulator_tests
	PRIVATE
	${GTEST_INCLUDE_DIRS}
	"${CMAKE_SOURCE_DIR}/src"
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