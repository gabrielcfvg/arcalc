cmake_minimum_required(VERSION 3.16)
project(arcalc)

set(CMAKE_CXX_STANDARD 17)

add_executable(arcalc main.cpp)


add_compile_options(-Wall -Wextra -pedantic)

add_executable(cteste main.cpp)
set(CMAKE_CXX_FLAGS_DEBUG "-g" CACHE STRING "Flags used by the CXX compiler during DEBUG builds" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG" CACHE STRING "Flags used by the CXX compiler during RELEASE builds" FORCE)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address" CACHE STRING "Flags used by the CXX compiler during ASAN builds" FORCE)
set(allowableBuildTypes Debug Release Asan)