cmake_minimum_required(VERSION 3.15)
project(aquarium C)

set(CMAKE_C_STANDARD 99)

add_executable(aquarium src/main.c src/aquarium.c src/display.c src/fishes.c
        src/object.c src/fish.c src/dl_list.c src/cartesian_point.c include/cartesian_point.h src/cartesian_velocity.c include/cartesian_velocity.h src/polar_point.c include/polar_point.h src/polar_velocity.c include/polar_velocity.h src/harmonic_movement.c include/harmonic_movement.h src/time_handler.c include/time_handler.h include/wall.h include/graphics_paths.h)
target_link_libraries(aquarium SDL2 SDL2_image m)