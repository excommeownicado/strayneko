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
    parser.c
    platform_posix.c
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

# display.c still contains a legacy event-loop implementation that defines
# ProcessNeko(). The actual game loop now lives in strayneko.c. Rename the
# legacy symbol at compile time until that old implementation is removed.
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
