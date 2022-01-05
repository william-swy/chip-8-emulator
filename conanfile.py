from conans import ConanFile


class CHIP8Emulator(ConanFile):
    name = "CHIP8Emulator"
    version = "0.2.0"
    requires = (
        "catch2/2.13.7",
        "sdl2/2.0.16@bincrafters/stable",
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"
