###################################################
# Thanks to Alexandru Naiman for optimization ideas
###################################################

cmake_minimum_required (VERSION 3.0)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE, DEBUG)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g2 -O0 -Wall -Werror")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -Wall -Werror -march=native -mtune=native -funroll-loops -Ofast -fno-math-errno -fomit-frame-pointer -foptimize-strlen -ftree-loop-distribution -ftree-loop-distribute-patterns -ffast-math -flto")
else(NOT MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_DEBUG /GS /Zi /debug")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /GS /GL /Gm /O2 /Od /Ox")
endif(NOT MSVC)

file (GLOB_RECURSE LiteEditorSourceFiles *)

add_library (LiteEditor SHARED ${LiteEditorSourceFiles})
set_property (TARGET LiteEditor PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories (LiteEditor PUBLIC ${PROJECT_SOURCE_DIR}/Editor)
target_include_directories (LiteEditor PRIVATE ${PROJECT_SOURCE_DIR}/Engine)
target_include_directories (LiteEditor PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty/include/glm)

if(NOT MSVC)
	find_package (Bullet REQUIRED)
	
	include_directories (${BULLET_INCLUDE_DIRS})
else(NOT MSVC)
	include_directories (${PROJECT_SOURCE_DIR}/3rdparty/include/bullet)
endif(NOT MSVC)

target_link_libraries (LiteEditor stdc++fs)

target_compile_definitions (LiteEditor PRIVATE GLM_ENABLE_EXPERIMENTAL)
