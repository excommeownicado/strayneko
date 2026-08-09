cmake_minimum_required(VERSION 3.16)
project(strayneko VERSION 1.0.2 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)

option(ENABLE_DEBUG "Enable debug logging support" OFF)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

include(GNUInstallDirs)

find_package(X11 REQUIRED)
find_library(XRANDR_LIBRARY NAMES Xrandr)
find_library(XEXT_LIBRARY NAMES Xext)

if(NOT XRANDR_LIBRARY)
    message(FATAL_ERROR "Could not find Xrandr library")
endif()

if(NOT XEXT_LIBRARY)
    message(FATAL_ERROR "Could not find Xext library")
endif()

option(ENABLE_TESTS "Build tests" OFF)

add_subdirectory(src)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(FILES docs/strayneko.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)

install(FILES resources/desktop/strayneko.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

install(FILES resources/desktop/strayneko.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
