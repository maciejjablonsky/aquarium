cmake_minimum_required(VERSION 3.15)
project(aquarium C)

set(CMAKE_C_STANDARD 99)

add_executable(aquarium main.c dl_list.c fish.c)
include(FindPkgConfig)
pkg_search_module(SDL2 required sdl2)
target_link_libraries(aquarium SDL2 SDL2_image m)