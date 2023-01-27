message("-- Fetching External Project: spirv_reflect")

FetchContent_Declare(
  spirv_reflect
  GIT_REPOSITORY  https://github.com/KhronosGroup/SPIRV-Reflect.git
  GIT_TAG         sdk-1.3.224.1
)

FetchContent_GetProperties(spirv_reflect)
if(NOT spirv_reflect_POPULATED)
  FetchContent_Populate(spirv_reflect)

  set(src_spirv_reflect
    "${spirv_reflect_SOURCE_DIR}/spirv_reflect.h"
    "${spirv_reflect_SOURCE_DIR}/spirv_reflect.c"
    "${spirv_reflect_SOURCE_DIR}/spirv_reflect.cpp")

  add_library(spirv_reflect STATIC ${src_spirv_reflect})

  target_include_directories(spirv_reflect SYSTEM PUBLIC 
    "${spirv_reflect_SOURCE_DIR}"
    "${spirv_reflect_SOURCE_DIR}/..")

  set_target_properties(spirv_reflect PROPERTIES FOLDER "thirdparty/spirv_reflect")
endif()