cmake_minimum_required (VERSION 3.6)

if (WIN32)
	set(CMAKE_SYSTEM_VERSION 10.0 (latest installed version))
endif ()

project (LiteEngine)

include_directories(/usr/local/include)

add_subdirectory (Tools)

add_subdirectory (3rdparty)

add_subdirectory (Engine)
add_subdirectory (Editor)
add_subdirectory (Templates)
