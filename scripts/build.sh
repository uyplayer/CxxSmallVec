# 安装依赖 (Debug 模式)
conan install . --build=missing -s build_type=Debug

# 生成构建文件 (使用 conan 生成的 toolchain / preset)
cmake --preset conan-debug

# 编译 (调用 conan-debug preset 下的构建目录)
cmake --build --preset conan-debug