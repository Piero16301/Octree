cmake_minimum_required(VERSION 3.16)
project(Octree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-lX11")
set(CMAKE_CXX_FLAGS "-lpthread")
set(CMAKE_CXX_FLAGS "-ljpeg")

add_executable(Octree main.cpp Octree.h Octree.cpp Node.h Node.cpp)

target_link_libraries(Octree -lX11 -lpthread -ljpeg)
