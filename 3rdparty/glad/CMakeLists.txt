cmake_minimum_required(VERSION 3.17)

project(GLAD)

add_library(GLAD STATIC src/glad.c)

target_include_directories(GLAD PUBLIC "include/")