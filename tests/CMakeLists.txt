add_executable(test_parser
    test_parser.c
    ../src/parser.c
)

target_include_directories(test_parser PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

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

add_test(
    NAME geometry
    COMMAND test_geometry
)
