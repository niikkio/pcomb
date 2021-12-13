include(FetchContent)
set(FETCHCONTENT_QUIET off)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # windows specific
FetchContent_MakeAvailable(googletest)

add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

file(GLOB TEST_SOURCE_FILES "*.cc")
add_executable(core_test ${TEST_SOURCE_FILES})

target_link_libraries(core_test PRIVATE gtest_main
                                PRIVATE gmock_main
                                PRIVATE pcomb)


target_include_directories(core_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) 

set_target_properties(core_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(GoogleTest)
gtest_discover_tests(core_test)
