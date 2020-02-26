cmake_minimum_required(VERSION 3.15)
project(aquarium C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "--coverage ")

find_package(LibXml2 REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${LIBXML2_INCLUDE_DIR})


add_executable(aquarium src/main.c src/aquarium.c src/display/display.c src/fishes/fishes.c
        src/object.c src/fishes/fish.c src/fishes/dl_list.c src/fishes/motion/cartesian_point.c src/fishes/motion/cartesian_point.h src/fishes/motion/cartesian_velocity.c
        src/fishes/motion/cartesian_velocity.h src/fishes/motion/polar_point.c src/fishes/motion/polar_point.h
        src/fishes/motion/polar_velocity.c src/fishes/motion/polar_velocity.h src/fishes/motion/harmonic_motion.c src/fishes/motion/harmonic_motion.h src/time_handler.c
        src/time_handler.h src/fishes/wall.h src/display/graphics_paths.h src/fishes/motion/translational_motion.c src/fishes/motion/translational_motion.h
        src/degrees.c src/degrees.h src/random.c src/random.h src/fishes/motion/harmonic_velocity.c src/fishes/motion/harmonic_velocity.h
         src/memory_handling.h src/config_parser/config_parser.c src/config_parser/config_parser.h)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(aquarium PUBLIC DEBUG)
endif()
target_link_libraries(aquarium SDL2 SDL2_image m ${LIBXML2_LIBRARIES})
add_subdirectory(tests)