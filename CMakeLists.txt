cmake_minimum_required(VERSION 3.15)
project(aquarium C)

set(CMAKE_C_STANDARD 99)

add_executable(aquarium main.c aquarium.c display.c fishes.c
        object.c fish.c dl_list.c cartesian_point.c cartesian_point.h cartesian_velocity.c cartesian_velocity.h polar_point.c polar_point.h polar_velocity.c polar_velocity.h harmonic_movement.c harmonic_movement.h time_handler.c time_handler.h wall.h)
target_link_libraries(aquarium SDL2 SDL2_image m)