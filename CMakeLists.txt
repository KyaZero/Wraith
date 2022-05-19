cmake_minimum_required(VERSION 3.21)

project("Wraith" VERSION "0.1.0" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "predefined")

include("cmake/compiler_features.cmake")
include("cmake/compiler_warnings.cmake")
include("cmake/auto_group_sources.cmake")

message("-- Finding Vulkan")
find_package(Vulkan REQUIRED)
find_library(SHADERC_LIB shaderc_combined $ENV{VULKAN_SDK}/Lib)
find_library(SHADERC_LIBD shaderc_combinedd $ENV{VULKAN_SDK}/Lib)

set(${PROJECT_NAME}_FOLDER
    ${PROJECT_NAME}
    CACHE STRING "The folder that ${PROJECT_NAME} projects will be placed in.")

option(${PROJECT_NAME}_BUILD_EDITOR
    "If the editor target should be built."
    OFF)

add_subdirectory("thirdparty")
add_subdirectory("engine")

if(${${PROJECT_NAME}_BUILD_EDITOR})
    add_subdirectory("editor")
    target_compile_definitions(Engine PUBLIC WITH_EDITOR)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "Editor")