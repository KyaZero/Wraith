message("-- Fetching External Project: dubu_rect_pack")
include(FetchContent)

FetchContent_Declare(
    dubu_rect_pack
    GIT_REPOSITORY  https://github.com/Husenap/dubu-rect-pack.git
    GIT_TAG         b7fb731
)

set(dubu_rect_pack_BUILD_TESTS OFF)
set(dubu_rect_pack_FOLDER "thirdparty/dubu_rect_pack" CACHE STRING "" FORCE)

FetchContent_MakeAvailable(dubu_rect_pack)