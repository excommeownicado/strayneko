add_executable(oneko
    main.c
    config.c
    display.c
    bed.c
    oneko.c
)

target_include_directories(oneko PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${X11_INCLUDE_DIR}
)

target_compile_definitions(oneko PRIVATE
    _DEFAULT_SOURCE
    SHAPE
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR ENABLE_DEBUG)
    target_compile_definitions(oneko PRIVATE ENABLE_DEBUG)
endif()

target_link_libraries(oneko PRIVATE
    ${X11_LIBRARIES}
    ${XRANDR_LIBRARY}
    ${XEXT_LIBRARY}
    m
)

install(TARGETS oneko RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
