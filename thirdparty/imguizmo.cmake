message("-- Fetching External Project: ImGuizmo")
include(FetchContent)

FetchContent_Declare(
  ImGuizmo
  GIT_REPOSITORY  https://github.com/CedricGuillemet/ImGuizmo.git
  GIT_TAG         1.83
  SOURCE_DIR      "${FETCHCONTENT_BASE_DIR}/ImGuizmo/ImGuizmo"
)

FetchContent_GetProperties(ImGuizmo)
if(NOT ImGuizmo_POPULATED)
  FetchContent_Populate(ImGuizmo)

  # for some reason ${ImGuizmo_SOURCE_DIR} is emtpy, so I have this bandaid for now.
  set(src_ImGuizmo
    "${FETCHCONTENT_BASE_DIR}/ImGuizmo/ImGuizmo/ImGuizmo.cpp"
    "${FETCHCONTENT_BASE_DIR}/ImGuizmo/ImGuizmo/ImGuizmo.h")

  add_library(ImGuizmo STATIC ${src_ImGuizmo})

  target_include_directories(ImGuizmo SYSTEM PUBLIC 
    "${FETCHCONTENT_BASE_DIR}/ImGuizmo/ImGuizmo"
    "${FETCHCONTENT_BASE_DIR}/ImGuizmo/ImGuizmo/..")

  set_target_properties(ImGuizmo PROPERTIES FOLDER "thirdparty/ImGuizmo")

  # ImGuizmo.cpp has '#include "imgui.h"', so we must link it to imgui so it compiles.
  target_link_libraries(ImGuizmo PUBLIC imgui)
endif()