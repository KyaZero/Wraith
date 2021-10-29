message("-- Fetching External Project: harfbuzz")
include(FetchContent)

FetchContent_Declare(
    harfbuzz
    GIT_REPOSITORY  https://github.com/harfbuzz/harfbuzz.git
    GIT_TAG         2.8.2
)

FetchContent_MakeAvailable(harfbuzz)

set_target_properties(harfbuzz PROPERTIES FOLDER "thirdparty/harfbuzz")
set_target_properties(harfbuzz-subset PROPERTIES FOLDER "thirdparty/harfbuzz")