###################################################
# Thanks to Alexandru Naiman for optimization ideas
###################################################

cmake_minimum_required (VERSION 3.0)

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
	add_compile_options(/MP)
endif(NOT MSVC)

file (GLOB_RECURSE LiteEngineSourceFiles *)

add_executable (LiteEngine ${LiteEngineSourceFiles})

set (OpenGL_GL_PREFERENCE GLVND)

set (LIBS_DIR ${PROJECT_SOURCE_DIR}/3rdparty/lib)
find_package (OpenGL REQUIRED)
find_library (GLEW NAMES GLEW glew32 HINTS ${LIBS_DIR})
find_library (SDL2 NAMES SDL2 HINTS ${LIBS_DIR})
find_library (SDL2_image NAMES SDL2_image HINTS ${LIBS_DIR})
find_library (SDL_sound NAMES SDL_sound HINTS ${LIBS_DIR})
find_library (assimp NAMES assimp HINTS ${LIBS_DIR})

include_directories (${PROJECT_SOURCE_DIR}/Engine)
include_directories (${PROJECT_SOURCE_DIR}/3rdparty/include/glm)

if(NOT MSVC)
	find_package (OpenAL REQUIRED)

	include_directories (${OPENAL_INCLUDE_DIR})
else(NOT MSVC)
	include_directories (${PROJECT_SOURCE_DIR}/3rdparty/include/AL)

	find_library (OPENAL_LIBRARY NAMES OpenAL OpenAL32 HINTS ${LIBS_DIR})
endif(NOT MSVC)

if(NOT MSVC)
	find_package (Bullet REQUIRED)
	
	include_directories (${BULLET_INCLUDE_DIRS})
else(NOT MSVC)
	include_directories (${PROJECT_SOURCE_DIR}/3rdparty/include/bullet)

	find_library (BULLET_LIBRARY_COLLISION NAMES BulletCollision HINTS ${LIBS_DIR})
	find_library (BULLET_LIBRARY_DYNAMICS NAMES BulletDynamics HINTS ${LIBS_DIR})
	find_library (BULLET_LIBRARY_SOFTBODY NAMES BulletSoftBody HINTS ${LIBS_DIR})
	find_library (BULLET_LIBRARY_LINEARMATH NAMES LinearMath HINTS ${LIBS_DIR})

	set (BULLET_LIBRARIES ${BULLET_LIBRARY_COLLISION} ${BULLET_LIBRARY_DYNAMICS} ${BULLET_LIBRARY_SOFTBODY} ${BULLET_LIBRARY_LINEARMATH})
endif(NOT MSVC)

if(MSVC)
	include_directories (${PROJECT_SOURCE_DIR}/3rdparty/include)
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

target_link_libraries (LiteEngine ${OPENGL_LIBRARIES} ${GLEW} ${SDL2} ${SDL2_image} ${SDL_SOUND_LIBRARIES} ${OPENAL_LIBRARY} ${assimp} ${BULLET_LIBRARIES})

if(NOT MSVC)
	target_link_libraries (LiteEngine dl)
endif(NOT MSVC)

if(NOT MSVC)
	target_compile_definitions (LiteEngine PRIVATE ENGINE_API=)
else(NOT MSVC)
	SET_TARGET_PROPERTIES(LiteEngine PROPERTIES ENABLE_EXPORTS TRUE)

	target_compile_definitions (LiteEngine PRIVATE "ENGINE_API=__declspec(dllexport)")
endif(NOT MSVC)

target_compile_definitions (LiteEngine PRIVATE GLM_ENABLE_EXPERIMENTAL)
