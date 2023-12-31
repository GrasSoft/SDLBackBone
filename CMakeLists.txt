cmake_minimum_required(VERSION 3.27)
project(SDLBonanza)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

add_executable(SDLBonanza main.cpp
        Texture.cpp
        Texture.h
        Button.cpp
        Button.h
        Timer.cpp
        Timer.h
        Sprite.cpp
        Sprite.h
        definitions.h
        Scene.cpp
        Scene.h)

target_link_libraries(SDLBonanza ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
