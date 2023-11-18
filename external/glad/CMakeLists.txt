cmake_minimum_required(VERSION 3.26)
project(glad)

add_library(glad include/glad/gl.h src/gl.c)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(glad PUBLIC include/)