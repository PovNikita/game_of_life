cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(gameoflife VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

if (CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()


if(WIN32)
    add_executable(gameoflife WIN32 main.cpp core.cpp core.h user_interface.cpp user_interface.h SDL_wrappler.h SDL_wrappler.cpp)
    #set (SDL2_DLL_DIR CACHE STRING "SDL2 shared library directory")
    #set (SDL2_TTF_DLL_DIR CACHE STRING "SDL2 TTF shared library directory")
    set (SDL2_DLL_DIR "C:/dev/game_of_life/game_of_life/SDL2")
    set (SDL2_TTF_DLL_DIR "C:/dev/game_of_life/game_of_life/SDL2")
    target_include_directories(gameoflife PUBLIC "${SDL2_DLL_DIR}/include/SDL2" "${SDL2_TTF_DLL_DIR}/include/SDL2")
    target_link_libraries(gameoflife "${SDL2_DLL_DIR}/lib/libSDL2.dll.a" 
                                        "${SDL2_TTF_DLL_DIR}/lib/libSDL2_ttf.dll.a"
                                        ${SYSTEM_LIBS})
    file(COPY "${CMAKE_SOURCE_DIR}/bin/SDL2.dll" DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY "${CMAKE_SOURCE_DIR}/bin/SDL2_ttf.dll" DESTINATION ${CMAKE_BINARY_DIR})
elseif(UNIX)
    add_executable(gameoflife main.cpp core.cpp core.h user_interface.cpp user_interface.h SDL_wrappler.h SDL_wrappler.cpp)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    target_link_libraries(gameoflife ${SDL2_LIBRARIES} SDL2_ttf)
endif()


file(COPY ${CMAKE_SOURCE_DIR}/arial.ttf DESTINATION ${CMAKE_BINARY_DIR})