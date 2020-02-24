cmake_minimum_required(VERSION 3.15)

find_package(LibXml2 REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBXML2_INCLUDE_DIR})

add_executable(test_traversing_with_path.out
        traversing_with_path.c ../../src/object.c)
#target_compile_definitions(test_traversing_with_path.out PUBLIC DEBUG)
target_link_libraries(test_traversing_with_path.out SDL2 SDL2_image m
        ${LIBXML2_LIBRARIES} criterion --debug=gdb)

add_executable(test_retrieving_value_from_nodes_with_xpath.out
        retrieving_value_from_nodes.c ../../src/object.c)
target_link_libraries(test_retrieving_value_from_nodes_with_xpath.out
        SDL2 SDL2_image m
        ${LIBXML2_LIBRARIES} criterion --debug=gdb)

add_executable(test_trimming_whitespace.out trimming_whitespace.c ../../src/object.c)
target_link_libraries(test_trimming_whitespace.out
        SDL2 SDL2_image m
        ${LIBXML2_LIBRARIES} criterion --debug=gdb)