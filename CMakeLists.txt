cmake_minimum_required(VERSION 3.14)
project(pcomb VERSION 0.0.1 DESCRIPTION "C++ Combinatory Parsing")
set(CMAKE_CXX_STANDARD 17)


# set Boost_INCLUDE_DIR and Boost_LIBRARY_DIR for local build
include(CMakeLists.txt.in OPTIONAL)
find_package(Boost REQUIRED)

set(PCOMB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/pcomb")

add_library(pcomb SHARED
    ${PCOMB_SOURCE_DIR}/string_stream.cc
)

set_target_properties(pcomb PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

target_include_directories(pcomb PRIVATE ${PCOMB_SOURCE_DIR}
                                 PUBLIC ${Boost_INCLUDE_DIR}
                                 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(pcomb PRIVATE ${Boost_LIBRARIES})

# The tests are not built by default. To build them, set the
# pcomb_build_tests option to ON. You can do it by specifying
# the -Dpcomb_build_tests=ON flag when running cmake.

option(pcomb_build_tests "Build all of pcomb's own tests." OFF)

if (pcomb_build_tests)
  enable_testing()
  add_subdirectory(test)
endif()
