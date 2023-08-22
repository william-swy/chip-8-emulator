from conans import ConanFile


class CHIP8Emulator(ConanFile):
    name = "CHIP8Emulator"
    version = "0.2.0"
    requires = (
        "catch2/2.13.7",
        "sdl2/2.0.12@bincrafters/stable",
    )
    generators = "cmake", "cmake_find_package"
