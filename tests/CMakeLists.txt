set(TEST_WARNING_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat=2
)

add_executable(test_parser
    test_parser.c
    ../src/parser.c
)

target_include_directories(test_parser PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_compile_options(test_parser PRIVATE ${TEST_WARNING_FLAGS})

add_test(
    NAME parser
    COMMAND test_parser
)

add_executable(test_geometry
    test_geometry.c
    ../src/geometry.c
)

target_include_directories(test_geometry PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_compile_options(test_geometry PRIVATE ${TEST_WARNING_FLAGS})

add_test(
    NAME geometry
    COMMAND test_geometry
)

add_executable(test_target_bounds
    test_target_bounds.c
    ../src/geometry.c
    ../src/target_bounds.c
)

target_include_directories(test_target_bounds PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_compile_options(test_target_bounds PRIVATE ${TEST_WARNING_FLAGS})

add_test(
    NAME target_bounds
    COMMAND test_target_bounds
)
