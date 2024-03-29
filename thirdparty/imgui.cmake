message("-- Fetching External Project: imgui")
include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY  https://github.com/ocornut/imgui.git
  GIT_TAG         cce307a2be4d26c34cce7d4844381e18cc49f656
  SOURCE_DIR      "${FETCHCONTENT_BASE_DIR}/imgui/imgui"
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)

  set(src_imgui
    "${imgui_SOURCE_DIR}/imconfig.h"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui.h"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_internal.h"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imstb_rectpack.h"
    "${imgui_SOURCE_DIR}/imstb_textedit.h"
    "${imgui_SOURCE_DIR}/imstb_truetype.h"
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h"
    "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp")

  add_library(imgui STATIC ${src_imgui})

  target_include_directories(imgui SYSTEM PUBLIC 
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/..")

  set_target_properties(imgui PROPERTIES FOLDER "thirdparty/imgui")
endif()