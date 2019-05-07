CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(Serial CXX)



set(SOURCES
        serial/settings.h
        serial/settings.cpp
        serial/device.h)

if(UNIX AND NOT APPLE)
    set(SOURCES ${SOURCES}
            serial/nix/device.cpp
            serial/nix/device.h)
endif()


add_library(Serial ${SOURCES})


set_property(TARGET Serial PROPERTY CXX_STANDARD 14)