cmake_minimum_required(VERSION 3.18)
set(PROJECT_NAME "Engine")
project(${PROJECT_NAME} C CXX)

set_property(GLOBAL PROPERTY CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "predefined")

include("cmake/compiler_features.cmake")
include("cmake/compiler_warnings.cmake")
# include("cmake/conan.cmake")
include("cmake/imgui.cmake")

add_subdirectory("engine-lib")
add_subdirectory("app")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT app)