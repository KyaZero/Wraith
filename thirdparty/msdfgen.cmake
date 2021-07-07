message("-- Fetching External Project: msdfgen")
include(FetchContent)

FetchContent_Declare(
    msdfgen
    GIT_REPOSITORY  https://github.com/Chlumsky/msdfgen.git
    GIT_TAG         3e4570f8afbe62dd3edb81d04e4b9ce63220f397
    SOURCE_DIR      "${FETCHCONTENT_BASE_DIR}/msdfgen/msdfgen"
)

option(MSDFGEN_BUILD_MSDFGEN_STANDALONE "" OFF)
option(MSDFGEN_INSTALL "" OFF)

FetchContent_MakeAvailable(msdfgen)