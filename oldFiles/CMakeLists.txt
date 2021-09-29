cmake_minimum_required(VERSION 3.17)
project(asgn1 C)

set(CMAKE_C_STANDARD 90)

add_executable(asign1 asgn1.c htable.c htable.h mylib.c mylib.h)

target_link_libraries(asign1 m)