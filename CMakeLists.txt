###################################################
# Thanks to Alexandru Naiman for optimization ideas
###################################################

cmake_minimum_required (VERSION 3.0)

project (LiteEngine)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE, DEBUG)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g2 -O0 -Wall -Werror")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -Wall -Werror -march=native -mtune=native -funroll-loops -Ofast -fno-math-errno -fomit-frame-pointer -foptimize-strlen -ftree-loop-distribution -ftree-loop-distribute-patterns -ffast-math -flto")

include_directories (${PROJECT_SOURCE_DIR}/Engine)
include_directories (${PROJECT_SOURCE_DIR}/3rdparty/include)

file (GLOB_RECURSE LiteEngineSourceFiles Engine/*)

add_executable (LiteEngine ${LiteEngineSourceFiles})

target_link_libraries (LiteEngine GL GLU GLEW SDL2 SDL2_image assimp)
