message("-- Fetching External Project: tinyobjloader")

FetchContent_Declare(
	tinyobjloader
	GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG        v2.0.0rc9
)

FetchContent_MakeAvailable(tinyobjloader)

set_target_properties(tinyobjloader PROPERTIES FOLDER "thirdparty/tinyobjloader")