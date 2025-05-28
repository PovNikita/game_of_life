cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(gameoflife VERSION 0.0.1 LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)

add_executable(gameoflife main.cpp core.cpp core.h user_interface.cpp user_interface.h SDL_wrappler.h SDL_wrappler.cpp)
target_link_libraries(gameoflife ${SDL2_LIBRARIES} SDL2_ttf)

file(COPY ${CMAKE_SOURCE_DIR}/arial.ttf DESTINATION ${CMAKE_BINARY_DIR})