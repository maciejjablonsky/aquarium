cmake_minimum_required(VERSION 3.15)
project(aquarium_tests C)

add_subdirectory(xml_parsing)


find_package(LibXml2 REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${LIBXML2_INCLUDE_DIR})


add_executable(libxml_sandbox libxml_sandbox/libxml_sandbox.c)
target_link_libraries(libxml_sandbox ${LIBXML2_LIBRARIES})
