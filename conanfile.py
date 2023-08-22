from conans import ConanFile


class CHIP8Emulator(ConanFile):
    name = "CHIP8Emulator"
    version = "0.2.0"
    requires = (
        "catch2/2.13.7",
        "sdl2/2.0.16@bincrafters/stable",
    )
    generators = "cmake", "cmake_find_package"

    def package_info(self):
        self.conf_info.define("tools.system.package_manager:mode", "install");
        self.conf_info.define("tools.system.package_manager:sudo", True)
