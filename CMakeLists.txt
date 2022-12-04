cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
set(PICO_SDK_FETCH_FROM_GIT 1)
include(pico_sdk_import.cmake)

project(LarrysPasswordHolder C CXX ASM)
add_executable(LarrysPasswordHolder
        larrys_password.cpp
        button.cpp
        usb_descriptors.c
        )

# Initialize the SDK
pico_sdk_init()

# pull in common dependencies
target_link_libraries(LarrysPasswordHolder pico_stdlib pico_multicore tinyusb_device tinyusb_board)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(LarrysPasswordHolder PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(LarrysPasswordHolder)

