# chip-8-emulator

A C++ emulator for Chip8

## Build instructions
It is recommended to use vcpkg as the package manager as it will easily resolve dependencies. Also an in source build is prohibited as it pollutes the source directory

With vcpkg
```sh
mkdir build
cmake -B./build -DCMAKE_TOOL_CHAIN_FILE=<vcpkg_install_dir/scripts/buildsystems/vcpkg.cmake>
```

Without vcpkg
```sh
mkdir build
cmake -B./build
```
However the caveat is that you must provide the dependencies. The dependencies can be found in `vcpkg.json`

## Run instructions
The binary `chip8_emulator` is the application that will run and should be used like so: `./chip8_emulator <path to rom to be loaded>`. The `rom` folder in the source directory provides some sample roms that can be tested out.

The binary `chip8_emulator_tests` is the test suite for the emulation logic and can be simply run like so: `./chip8_emulator_tests`. All tests should pass.