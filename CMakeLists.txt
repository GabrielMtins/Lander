cmake_minimum_required(VERSION 3.5)
project(Lander LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(GAME_DIR "${PROJECT_SOURCE_DIR}/game")
set(ENGINE_DIR "${PROJECT_SOURCE_DIR}/engine")

add_subdirectory(external/glad)
add_subdirectory(external/r_json)

file(GLOB all_SRCS
		"${ENGINE_DIR}/src/*.c"
		"${GAME_DIR}/src/*.c"
        )

add_executable(${PROJECT_NAME} ${all_SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall
    -Wextra
    -Wpedantic
)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)

find_library(OPENGL OpenGL)
find_library(MATH_LIBRARY m)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
	include/
	external/glad/include
	external/r_json
	"${GAME_DIR}/include"
	"${ENGINE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
	${MATH_LIBRARY}
	${OPENGL}
	glad
	r_json
)
