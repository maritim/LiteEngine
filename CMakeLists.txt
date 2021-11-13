cmake_minimum_required (VERSION 3.6)

project (LiteEngine)

include_directories(/usr/local/include)

add_subdirectory (Tools)

add_subdirectory (3rdparty)

add_subdirectory (Engine)
add_subdirectory (Editor)
add_subdirectory (Templates)
