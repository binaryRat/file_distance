cmake_minimum_required(VERSION 3.14)
project(filedistance C)

set(CMAKE_C_STANDARD 11)

add_executable(filedistance main.c computedistance.c changefile.c directoryscan.c filereader.c)
