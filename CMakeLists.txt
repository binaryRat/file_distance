cmake_minimum_required(VERSION 3.14)
project(filedistance C)

set(CMAKE_C_STANDARD 11)

add_executable(filedistance main.c fdistance.c fapply.c dirscan.c utils.c)
