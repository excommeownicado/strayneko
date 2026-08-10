add_executable(strayneko
    main.c
    config.c
    resources.c
    colors.c
    geometry.c
    monitor.c
    display.c
    bed.c
    strayneko.c
    process_loop.c
    parser.c
    platform_posix.c
    platform_time_posix.c
    platform_input.c
    x11_window.c
    x11_event.c
    x11_platform_events.c
)

target_include_directories(strayneko PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/resources
    ${X11_INCLUDE_DIR}
)

target_compile_definitions(strayneko PRIVATE
    _DEFAULT_SOURCE
    SHAPE
)

# The legacy ProcessNeko() in strayneko.c is kept temporarily while the
# platform-independent process loop lives in process_loop.c.
set_source_files_properties(strayneko.c PROPERTIES
    COMPILE_DEFINITIONS "ProcessNeko=LegacyProcessNeko"
)

# display.c still contains the old event-loop implementation. Rename its
# ProcessNeko() until the legacy display loop is removed completely.
set_source_files_properties(display.c PROPERTIES
    COMPILE_DEFINITIONS "ProcessNeko=DisplayProcessNeko"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR ENABLE_DEBUG)
    target_compile_definitions(strayneko PRIVATE ENABLE_DEBUG)
endif()

target_compile_options(strayneko PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat=2
)

if(ENABLE_SANITIZERS)
    target_compile_options(strayneko PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    target_link_options(strayneko PRIVATE
        -fsanitize=address,undefined
    )
endif()

target_link_libraries(strayneko PRIVATE
    ${X11_LIBRARIES}
    ${XRANDR_LIBRARY}
    ${XEXT_LIBRARY}
    m
)

install(TARGETS strayneko RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
