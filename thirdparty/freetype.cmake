message("-- Fetching External Project: freetype")
include(FetchContent)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY  https://github.com/freetype/freetype.git
    GIT_TAG         50d0033f7ee600c5f5831b28877353769d1f7d48
)


option(CMAKE_DISABLE_FIND_PACKAGE_ZLIB "" TRUE)
option(CMAKE_DISABLE_FIND_PACKAGE_BZip2 "" TRUE)
option(CMAKE_DISABLE_FIND_PACKAGE_PNG "" TRUE)
option(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz "" TRUE)
option(CMAKE_DISABLE_FIND_PACKAGE_BrotliDec "" TRUE)

FetchContent_MakeAvailable(freetype)

add_library(Freetype::Freetype ALIAS freetype)

set_target_properties(freetype PROPERTIES FOLDER "thirdparty/freetype")