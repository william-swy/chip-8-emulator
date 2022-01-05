find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

# Compiles a static lib from the Catch2 header to shorten the entire build process
add_library(catch_main STATIC "catch_main.cpp")
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
target_link_libraries(catch_main PRIVATE project_options project_warnings)

# Test suite
add_executable(tests "memory_test.cpp" "cpu_test.cpp" "opcode_test.cpp" "graphics_test.cpp"
                 "keypad_test.cpp")

target_link_libraries(tests PRIVATE catch_main project_options project_warnings Arch)


catch_discover_tests(
  tests
  TEST_PREFIX
  "unittests."
  REPORTER
  xml
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests."
  OUTPUT_SUFFIX
  .xml)
