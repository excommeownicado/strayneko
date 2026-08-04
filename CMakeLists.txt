cmake_minimum_required(VERSION 3.16)
project(strayneko VERSION 1.2 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)

option(ENABLE_DEBUG "Enable debug logging support" OFF)

if(NOT CMAKE_C_FLAGS_DEBUG)
    set(CMAKE_C_FLAGS_DEBUG "-g")
endif()
if(NOT CMAKE_C_FLAGS_RELEASE)
    set(CMAKE_C_FLAGS_RELEASE "-O3")
endif()

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

add_subdirectory(src)

install(FILES strayneko.man DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
