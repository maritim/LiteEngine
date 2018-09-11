###################################################
# Thanks to Alexandru Naiman for optimization ideas
###################################################

cmake_minimum_required (VERSION 3.0)

project (LiteEngine)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE, DEBUG)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 11)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g2 -O0 -Wall -Werror")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -Wall -Werror -march=native -mtune=native -funroll-loops -Ofast -fno-math-errno -fomit-frame-pointer -foptimize-strlen -ftree-loop-distribution -ftree-loop-distribute-patterns -ffast-math -flto")
else(NOT MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_DEBUG /GS /Zi /debug")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /GS /GL /Gm /O2 /Od /Ox")
endif(NOT MSVC)

file (GLOB_RECURSE LiteEngineSourceFiles Engine/*)

add_executable (LiteEngine ${LiteEngineSourceFiles})

set (LIBS_DIR ${PROJECT_SOURCE_DIR}/3rdparty/lib)
find_package (OpenGL REQUIRED)
find_package (Bullet REQUIRED)
find_library (GLEW NAMES GLEW glew32 HINTS ${LIBS_DIR})
find_library (SDL2 NAMES SDL2 HINTS ${LIBS_DIR})
find_library (SDL2_image NAMES SDL2_image HINTS ${LIBS_DIR})
find_library (assimp NAMES assimp HINTS ${LIBS_DIR})

include_directories (${PROJECT_SOURCE_DIR}/Engine)

if(NOT MSVC)
	include_directories (${BULLET_INCLUDE_DIRS})
else(NOT MSVC)
	include_directories (${PROJECT_SOURCE_DIR}/3rdparty/include)
endif(NOT MSVC)

target_link_libraries (LiteEngine ${OPENGL_LIBRARIES} ${GLEW} ${SDL2} ${SDL2_image} ${assimp} ${BULLET_LIBRARIES})
