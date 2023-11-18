cmake_minimum_required(VERSION 3.27)
project(SDLBonanza)

set(CMAKE_CXX_STANDARD 23)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(SDLBonanza main.cpp)
target_link_libraries(SDLBonanza ${SDL2_LIBRARIES})
