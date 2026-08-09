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

target_compile_definitions(test_geometry PRIVATE
    _DEFAULT_SOURCE
    SHAPE
)

target_include_directories(test_geometry PRIVATE
    ${X11_INCLUDE_DIR}
)

target_link_libraries(test_geometry PRIVATE
    ${X11_LIBRARIES}
    ${XRANDR_LIBRARY}
    ${XEXT_LIBRARY}
    m
)

add_test(
    NAME geometry
    COMMAND test_geometry
)
