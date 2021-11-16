###################################################
# Thanks to Alexandru Naiman for optimization ideas
###################################################

cmake_minimum_required (VERSION 3.6)

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE, DEBUG)
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 17)

if(NOT MSVC)
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g2 -O0 -Wall -Werror")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -Wall -Werror -march=native -mtune=native -funroll-loops -Ofast -fno-math-errno -fomit-frame-pointer -foptimize-strlen -ftree-loop-distribution -ftree-loop-distribute-patterns -ffast-math -flto")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR APPLE)
		set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-overloaded-virtual")
		set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-overloaded-virtual")
		if(APPLE)
			set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-deprecated-declarations")
			set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-deprecated-declarations")
		endif(APPLE)
	endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR APPLE)
else(NOT MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /D_DEBUG /GS /Zi /debug")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /nologo /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS /GS /GL /Gm /O2 /Od /Ox")
	add_compile_options(/MP)
endif(NOT MSVC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file (GLOB_RECURSE LiteEngineSourceFiles *)

file (GLOB_RECURSE LiteEngineComponentHeaderFiles Components/*.h)

add_custom_command(
    COMMAND LiteHeaderTool Components ${CMAKE_CURRENT_BINARY_DIR}/GeneratedComponents.cpp ENGINE_API
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/GeneratedComponents.cpp
    DEPENDS ${LiteEngineComponentHeaderFiles} LiteHeaderTool
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Engine
    COMMENT "Generate persistent component data..."
    )

set (GeneratedSourceFiles ${CMAKE_CURRENT_BINARY_DIR}/GeneratedComponents.cpp)

add_executable (LiteEngine ${LiteEngineSourceFiles} ${GeneratedSourceFiles})

if (MSVC)
    set_property(DIRECTORY "${PROJECT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT LiteEngine)
    set_property(TARGET LiteEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()

set (OpenGL_GL_PREFERENCE GLVND)

find_package (OpenGL REQUIRED)
find_library (GLEW NAMES GLEW glew32 HINTS ${PROJECT_SOURCE_DIR}/3rdparty/GLEW/lib)
find_library (SDL2 NAMES SDL2 HINTS ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/lib)
find_library (SDL2_image NAMES SDL2_image HINTS ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/lib)
find_library (SDL_sound NAMES SDL_sound HINTS ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/lib)

target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/Engine)
target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/ImGui)
target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/glm)
target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/spdlog)
target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/SimpleIni)
target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/bullet/src)
target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/assimp/include)

# OpenAL

if(NOT MSVC)
	find_package (OpenAL REQUIRED)

	target_include_directories (LiteEngine PUBLIC ${OPENAL_INCLUDE_DIR})
else(NOT MSVC)
	target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/OpenAL/include)

	find_library (OPENAL_LIBRARY NAMES OpenAL OpenAL32 HINTS ${PROJECT_SOURCE_DIR}/3rdparty/OpenAL/lib)
endif(NOT MSVC)

if(MSVC)
	target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/OpenAL/include)
	target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/GLEW/include)
	target_include_directories (LiteEngine PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/include)
endif(MSVC)

if(MSVC)
	MACRO(GROUP_SRC SRCS)
	  foreach(FILE ${LiteEngineSourceFiles}) 
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

	group_src (${LiteEngineSourceFiles})
endif(MSVC)

# On Windows, we copy required binary files next to the target.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(TARGET LiteEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/3rdparty/GLEW/bin/glew32.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/bin/libjpeg-9.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/bin/libpng16-16.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/bin/libtiff-5.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/bin/libwebp-4.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/OpenAL/bin/OpenAL32.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/bin/SDL2.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/bin/SDL2_image.dll
            ${PROJECT_SOURCE_DIR}/3rdparty/SDL2/bin/zlib1.dll
			$<TARGET_FILE:ImGui>
			$<TARGET_FILE:SimpleIni>

            # Destination directory
            $<TARGET_FILE_DIR:LiteEngine>
    )
endif()

target_link_libraries (LiteEngine 
	${OPENGL_LIBRARIES}
	${GLEW}
	${SDL2}
	${SDL2_image}
	${SDL_SOUND_LIBRARIES}
	${OPENAL_LIBRARY}
	assimp
	BulletCollision
	BulletDynamics
	BulletSoftBody
	LinearMath
	ImGui
	SimpleIni
)

if(NOT MSVC)
	target_link_libraries (LiteEngine dl)
endif(NOT MSVC)

SET_TARGET_PROPERTIES(LiteEngine PROPERTIES ENABLE_EXPORTS TRUE)

if(NOT MSVC)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		target_link_libraries (LiteEngine stdc++fs)
	endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	target_compile_definitions (LiteEngine PRIVATE ENGINE_API=)
else(NOT MSVC)
	target_compile_definitions (LiteEngine PRIVATE "ENGINE_API=__declspec(dllexport)")
endif(NOT MSVC)

target_compile_definitions (LiteEngine PRIVATE GLM_ENABLE_EXPERIMENTAL)
