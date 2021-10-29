message("-- Fetching External Project: fribidi")
include(FetchContent)

FetchContent_Declare(
  fribidi
  GIT_REPOSITORY  https://github.com/Husenap/fribidi.git
  GIT_TAG         c80a4616e0e16cb97a6b3197fe138bc23fc01688
)

FetchContent_MakeAvailable(fribidi)

set_target_properties(fribidi PROPERTIES FOLDER "thirdparty/fribidi")