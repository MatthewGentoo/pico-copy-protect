cmake_minimum_required(VERSION 3.13)

include(pico-sdk/pico_sdk_init.cmake)

project(pico-copy-protect)

pico_sdk_init()

add_executable(demo main.c)
target_link_libraries(demo hardware_flash pico_stdlib pico_unique_id)
pico_add_extra_outputs(demo)
