cmake_minimum_required(VERSION 3.10)
project(aquarium C)

set(CMAKE_C_STANDARD 11)

add_executable(aquarium main.c fish.c sl_list.c)
target_link_libraries(aquarium SDL2 SDL2_image m)