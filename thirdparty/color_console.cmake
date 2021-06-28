message("-- Fetching External Project: color-console")

FetchContent_Declare(color-console
	GIT_REPOSITORY https://github.com/imfl/color-console
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
)

FetchContent_GetProperties(color-console)
if (NOT color-console_POPULATED)
	FetchContent_Populate(color-console)
endif()

add_library(color-console INTERFACE)
target_include_directories(color-console INTERFACE ${color-console_SOURCE_DIR}/include)