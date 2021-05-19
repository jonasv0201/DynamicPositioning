cmake_minimum_required(VERSION 3.10)
project(boat C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories("/usr/local/lib/Phidget22")
find_library(phidget22 "/usr/local/lib/Phidget21/phidget22.h")


add_executable(dynamic

        src/phidget.c
        src/main.h
        src/gnuplot.c
        src/main.c
        src/mainSub.c
        )

target_link_libraries(dynamic phidget22)
