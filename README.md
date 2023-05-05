# chip-8-emulator
[![Build status](https://ci.appveyor.com/api/projects/status/en14124l452s7jbx?svg=true)](https://ci.appveyor.com/project/william-swy/chip-8-emulator)

A C++ emulator for Chip8

## Samples
![pong](samples/Pong.gif)
Figure 1. The game pong

## Keyboard Bindings

Original Chip 8 Key Set
| 1 | 2 | 3 | C |
|:-:|:-:|:-:|:-:|
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

Key Set Mapping To A Modern Keyboard
| 1 | 2 | 3 | 4 |
|:-:|:-:|:-:|:-:|
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |


## Build instructions
It is recommended to use [Conan](https://github.com/conan-io/conan) as the package manager as it will easily resolve dependencies.
A vcpkg option is also available. Requires Conan version >= 1.54 but < 2.0

With Conan
```sh
mkdir build
cmake -B./build -DPACKAGE_MANAGER=Conan
```

With vcpkg
```sh
mkdir build
cmake -B./build -DPACKAGE_MANAGER=vcpkg -DCMAKE_TOOL_CHAIN_FILE=<vcpkg_install_dir/scripts/buildsystems/vcpkg.cmake>
```

Without package manager
```sh
mkdir build
cmake -B./build
```
However the caveat is that you must provide the dependencies. The dependencies can be found in `vcpkg.json` or `conanfile.py`

## Run instructions
The binary `chip8_emulator` is the application that will run and should be used like so: `./chip8_emulator <path to rom to be loaded>`. The `rom` folder in the source directory provides some sample roms that can be tested out.

The binary `chip8_emulator_tests` is the test suite for the emulation logic and can be simply run like so: `./chip8_emulator_tests`. All tests should pass.

## References
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#8xy2 gives a streight forward explaination of the Chip8 architecture
- https://github.com/bradford-hamilton/chippy/blob/master/roms/ibm_logo.ch8 for the IBM logo rom
- https://github.com/bradford-hamilton/chippy/blob/master/roms/chip8_logo.ch8 for the chip8_logo rom
- https://github.com/bradford-hamilton/chippy/blob/master/roms/pong.ch8 for the pong game rom
- https://github.com/mattmikolay/chip-8/blob/master/cavern/cavern.ch8 for the cavern game rom
