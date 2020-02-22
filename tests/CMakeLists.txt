cmake_minimum_required(VERSION 3.15)
project(aquarium_tests C)

find_package(LibXml2 REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBXML2_INCLUDE_DIR})

set(XML_PARSING_DIR xml_parsing)

add_executable(test_retrieving_node_name_from_path ${XML_PARSING_DIR}/retrieving_node_name_from_path.c ../src/object.c)
target_link_libraries(test_retrieving_node_name_from_path SDL2 SDL2_image m ${LIBXML2_LIBRARIES} criterion "--debug=gdb")


add_executable(test_skip_slashes ${XML_PARSING_DIR}/skip_slashes_in_path.c ../src/object.c)
target_link_libraries(test_skip_slashes SDL2 SDL2_image m ${LIBXML2_LIBRARIES} criterion "--debug=gdb")
