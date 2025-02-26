cmake_minimum_required(VERSION 3.2)

project(evlve_pbg)

set(CMAKE_C_COMPILER /usr/bin/gcc-11)
set(CMAKE_CXX_COMPILER /usr/bin/g++-11)

# add_executable(${CMAKE_PROJECT_NAME} dph_eq_solver.cpp dph_eq_solver.h)
add_executable(${CMAKE_PROJECT_NAME} ant_postman.cpp ant_postman.h)