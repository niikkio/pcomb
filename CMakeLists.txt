cmake_minimum_required(VERSION 3.14)
project(pcomb VERSION 0.0.1 DESCRIPTION "C++ Combinatory Parsing")
set(CMAKE_CXX_STANDARD 17)

message("${CMAKE_CURRENT_SOURCE_DIR}")

set(PCOMB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/pcomb")
set(PCOMB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/pcomb")

file(GLOB PCOMB_SOURCE_FILES "${PCOMB_SOURCE_DIR}/*.cc")

add_library(pcomb SHARED ${PCOMB_SOURCE_FILES})
set_target_properties(pcomb PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_include_directories(pcomb PRIVATE ${PCOMB_SOURCE_DIR}
                                 PRIVATE ${PCOMB_INCLUDE_DIR}
                                 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()
add_subdirectory(test)
