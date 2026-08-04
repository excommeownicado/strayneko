cmake_minimum_required(VERSION 3.16)
project(oneko VERSION 1.2 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

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

install(FILES oneko.man DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
