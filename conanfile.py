from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout



class CxxSmallVec(ConanFile):
    name = "CxxSmallVec"
    version = "1.0.0"
    author = "uyplayer@outlook.com"
    description = "Thread-safe lazy initialization for modern C++ | 现代 C++ 的线程安全惰性初始化"
    settings = "os", "compiler", "build_type", "arch"

 
    generators = "CMakeDeps", "CMakeToolchain"

 
    def requirements(self):
        self.requires("spdlog/1.15.1")
 
        

 
    def layout(self):
        cmake_layout(self)
 
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
 