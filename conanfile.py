from conans import ConanFile


class CHIP8Emulator(ConanFile):
    name = "CHIP8Emulator"
    version = "0.2.0"
    requires = (
        "catch2/2.13.7",
        "sdl/2.26.1@#152c43fdd07f187f0b2a9150596c9c63",
    )
    generators = "cmake", "cmake_find_package"
