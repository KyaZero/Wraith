message("-- Fetching External Project: freetype")
include(FetchContent)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY  https://github.com/freetype/freetype.git
    GIT_TAG         50d0033f7ee600c5f5831b28877353769d1f7d48
)


set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_BZip2 TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_PNG TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE CACHE BOOL "" FORCE)
set(CMAKE_DISABLE_FIND_PACKAGE_BrotliDec TRUE CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(freetype)

add_library(Freetype::Freetype ALIAS freetype)