message("-- Fetching External Project: color-console")

FetchContent_Declare(
  color-console
  GIT_REPOSITORY    https://github.com/imfl/color-console
  GIT_TAG           745f57141e6322e20cdda59a74ab1e00d107ade1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
)

FetchContent_GetProperties(color-console)
if (NOT color-console_POPULATED)
	FetchContent_Populate(color-console)

  add_library(color-console INTERFACE)
  target_include_directories(color-console INTERFACE ${color-console_SOURCE_DIR}/include)
endif()
