set(target_name "Editor")

set(panel_files
    "src/Panels/BarPanel.cpp"
    "src/Panels/BarPanel.h"
    "src/Panels/Panel.cpp"
    "src/Panels/Panel.h"
    "src/Panels/PanelManager.cpp"
    "src/Panels/PanelManager.h"
    "src/Panels/PropertiesPanel.cpp"
    "src/Panels/PropertiesPanel.h"
    "src/Panels/SceneHierarchyPanel.cpp"
    "src/Panels/SceneHierarchyPanel.h"
    "src/Panels/ViewportPanel.cpp"
    "src/Panels/ViewportPanel.h")

set(src_files
    "src/Editor.h"
    "src/Editor.cpp"
    "src/main.cpp")

set(all_files
    ${src_files}
    ${panel_files})
    
add_executable(${target_name} ${all_files})

target_link_libraries(${target_name}
    compiler_features
    compiler_warnings
    Engine
    imgui)

auto_group_sources(all_files ${CMAKE_CURRENT_SOURCE_DIR})
source_group("src/Precompiled" REGULAR_EXPRESSION "cmake_pch\.[ch]xx")

set_target_properties(${target_name} PROPERTIES
    FOLDER ${PROJECT_NAME}
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)