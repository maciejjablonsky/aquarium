cmake_minimum_required(VERSION 3.15)
project(aquarium C)

set(CMAKE_C_STANDARD 99)

add_executable(aquarium src/main.c src/aquarium.c src/display.c src/fishes.c
        src/object.c src/fish.c src/dl_list.c src/cartesian_point.c src/cartesian_point.h src/cartesian_velocity.c
        src/cartesian_velocity.h src/polar_point.c src/polar_point.h
        src/polar_velocity.c src/polar_velocity.h src/harmonic_motion.c src/harmonic_motion.h src/time_handler.c
        src/time_handler.h src/wall.h src/graphics_paths.h src/translational_motion.c src/translational_motion.h src/degrees.c src/degrees.h src/random.c src/random.h src/harmonic_velocity.c src/harmonic_velocity.h)
target_link_libraries(aquarium SDL2 SDL2_image m)