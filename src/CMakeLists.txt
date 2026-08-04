add_executable(strayneko
    main.c
    config.c
    display.c
    bed.c
    strayneko.c
)

target_include_directories(strayneko PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${X11_INCLUDE_DIR}
)

target_compile_definitions(strayneko PRIVATE
    _DEFAULT_SOURCE
    SHAPE
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR ENABLE_DEBUG)
    target_compile_definitions(strayneko PRIVATE ENABLE_DEBUG)
endif()

target_link_libraries(strayneko PRIVATE
    ${X11_LIBRARIES}
    ${XRANDR_LIBRARY}
    ${XEXT_LIBRARY}
    m
)

install(TARGETS strayneko RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
