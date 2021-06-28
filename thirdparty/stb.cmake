message("-- Fetching External Project: stb")

FetchContent_Declare(stb_image
  GIT_REPOSITORY https://github.com/nothings/stb
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_GetProperties(stb_image)
if (NOT stb_image)
    FetchContent_Populate(stb_image)
endif()

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})