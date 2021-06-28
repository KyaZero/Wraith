cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME "2DGE")
project(${PROJECT_NAME} C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "predefined")

include("cmake/compiler_features.cmake")
include("cmake/compiler_warnings.cmake")

add_subdirectory("thirdparty")

add_subdirectory("editor")
add_subdirectory("engine")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Editor")