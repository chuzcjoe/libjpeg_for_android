#!/bin/bash

# Define variables
build_dir="build"
device_dir="/data/local/tmp/jpeg_demo"
img_dir="imgs"

# Clean and create build directory
rm -rf "${build_dir}"
mkdir -p "${build_dir}"
cd "${build_dir}" || exit

# Run cmake and make
cmake ..
make

# Go back to the root directory
cd ..

# Prepare the device directory
adb shell "rm -rf ${device_dir}"
adb shell "mkdir -p ${device_dir}"

# Push the executable to the device
adb push "${build_dir}/MyTurboJPEGProject" "${device_dir}"

# Push all images to the device directory
adb push "${img_dir}/." "${device_dir}/"

# Change the permissions of the executable to make it executable
adb shell "chmod +x ${device_dir}/MyTurboJPEGProject"

# Run the executable on the device
adb shell "cd ${device_dir} && ./MyTurboJPEGProject input.jpg save.jpg"

adb pull "${device_dir}/save.jpg" "./"
