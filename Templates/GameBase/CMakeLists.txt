###################################################
# Thanks to Alexandru Naiman for optimization ideas
###################################################

cmake_minimum_required (VERSION 3.6)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE, DEBUG)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g2 -O0 -Wall -Werror -fPIC")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -Wall -Werror -fPIC -funroll-loops -Ofast -fno-math-errno -fomit-frame-pointer -ffast-math -flto")

	if (NOT CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm")
		set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native")
	endif ()
else(NOT MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_DEBUG /GS /Zi /debug")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /GS /GL /Gm /O2 /Od /Ox")
	add_compile_options(/MP)
endif(NOT MSVC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file (GLOB_RECURSE GameSourceFiles *)

file (GLOB_RECURSE GameComponentHeaderFiles Components/*.h)

add_custom_command(
    COMMAND LiteHeaderTool Components ${CMAKE_CURRENT_BINARY_DIR}/GeneratedComponents.cpp GAME_API
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/GeneratedComponents.cpp
    DEPENDS ${GameComponentHeaderFiles} LiteHeaderTool
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Templates/GameBase
    COMMENT "Generate persistent component data..."
    )

set (GeneratedSourceFiles ${CMAKE_CURRENT_BINARY_DIR}/GeneratedComponents.cpp)

add_library (GameBase SHARED ${GameSourceFiles} ${GeneratedSourceFiles})
set_property (TARGET GameBase PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories (GameBase PUBLIC ${PROJECT_SOURCE_DIR}/Templates/GameBase)
target_include_directories (GameBase PRIVATE ${PROJECT_SOURCE_DIR}/Engine)
target_include_directories (GameBase PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/ImGui)
target_include_directories (GameBase PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/glm)
target_include_directories (GameBase PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/SimpleIni)
target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/bullet/src)

target_link_libraries (GameBase ImGui)

if(MSVC)
	target_include_directories (GameBase PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/include)
endif(MSVC)

if(MSVC)
	MACRO(GROUP_SRC SRCS)
	  foreach(FILE ${GameSourceFiles}) 
		#convert source file to absolute
		get_filename_component(ABSOLUTE_PATH "${FILE}" ABSOLUTE)
		# Get the directory of the absolute source file
		get_filename_component(PARENT_DIR "${ABSOLUTE_PATH}" DIRECTORY)
		# Remove common directory prefix to make the group
		string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
		# Make sure we are using windows slashes
		string(REPLACE "/" "\\" GROUP "${GROUP}")
		# Group into "Source Files" and "Header Files"
		if ("${FILE}" MATCHES ".*\\.cpp")
	      set(GROUP "Source Files${GROUP}")
	    elseif("${FILE}" MATCHES ".*\\.h")
	      set(GROUP "Header Files${GROUP}")
	    endif()
	    source_group("${GROUP}" FILES "${FILE}")
	  endforeach()
	ENDMACRO(GROUP_SRC)

	group_src (${GameSourceFiles})
endif(MSVC)

target_link_libraries (GameBase LiteEngine)

if(NOT MSVC)
	target_compile_definitions (GameBase PRIVATE ENGINE_API=)
	target_compile_definitions (GameBase PRIVATE GAME_API=)
else(NOT MSVC)
	target_compile_definitions (GameBase PRIVATE "ENGINE_API=__declspec(dllimport)")
	target_compile_definitions (GameBase PRIVATE "GAME_API=__declspec(dllexport)")
endif(NOT MSVC)

if(APPLE)
	target_link_options(GameBase PRIVATE -undefined dynamic_lookup)
endif(APPLE)

target_compile_definitions (GameBase PRIVATE GLM_ENABLE_EXPERIMENTAL)
