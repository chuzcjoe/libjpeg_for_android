cmake_minimum_required(VERSION 3.10)

# Set Android specific variables
set(ANDROID_ABI arm64-v8a)
set(ANDROID_PLATFORM android-21)
set(ANDROID_TOOLCHAIN clang)
set(CMAKE_TOOLCHAIN_FILE /Users/zongchengchu/Library/Android/sdk/ndk/25.1.8937393/build/cmake/android.toolchain.cmake)

# Set CMake ASM flags
set(CMAKE_ASM_FLAGS "--target=aarch64-linux-android21")

project(MyTurboJPEGProject)

# Set the path to the TurboJPEG include directory and library
set(TURBOJPEG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TURBOJPEG_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libturbojpeg.a")

# Include the TurboJPEG headers
include_directories(${TURBOJPEG_INCLUDE_DIR})

# Add your executable
add_executable(MyTurboJPEGProject main.cpp)

# Link against the TurboJPEG library
target_link_libraries(MyTurboJPEGProject ${TURBOJPEG_LIBRARY})

