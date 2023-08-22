from sys import platform

from conans import ConanFile


class CHIP8Emulator(ConanFile):
    name = "CHIP8Emulator"
    version = "0.2.0"
    generators = "cmake", "cmake_find_package"

    def config_options(self):
        self.conf_info.define("tools.system.package_manager:mode", "install")
        self.conf_info.define("tools.system.package_manager:sudo", True)

    def requirements(self):
        self.requires("catch2/2.13.7")
        self.requires("sdl2/2.0.16@bincrafters/stable")
        if platform == "linux" or platform == "linux2":
            self.requires("libiconv/1.17", override=True)
