cmake_minimum_required(VERSION 3.16)
project(arcalc)

set(CMAKE_CXX_STANDARD 17)

add_executable(arcalc main.cpp)

find_package(fmt)

target_link_libraries(arcalc PRIVATE fmt::fmt)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(cteste main.cpp)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address" CACHE STRING "Flags used by the CXX compiler during DEBUG builds" FORCE)
