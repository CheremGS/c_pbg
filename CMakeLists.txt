cmake_minimum_required(VERSION 3.6)
project(evolve_pubg)

set(SRC_LIST c_version.cpp dph_eq_solver.h dph_eq_solver.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})