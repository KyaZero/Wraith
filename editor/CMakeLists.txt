set(target_name "Editor")

set(panel_files
    "src/Panels/Debug/DemoPanel.cpp"
    "src/Panels/Debug/DemoPanel.h"
    "src/Panels/Debug/ThreadSchedulerPanel.cpp"
    "src/Panels/Debug/ThreadSchedulerPanel.h"
    "src/Panels/Play/BarPanel.cpp"
    "src/Panels/Play/BarPanel.h"
    "src/Panels/Play/ViewportPanel.cpp"
    "src/Panels/Play/ViewportPanel.h"
    "src/Panels/Scene/PropertiesPanel.cpp"
    "src/Panels/Scene/PropertiesPanel.h"
    "src/Panels/Scene/SceneHierarchyPanel.cpp"
    "src/Panels/Scene/SceneHierarchyPanel.h"
    "src/Panels/Panel.cpp"
    "src/Panels/Panel.h"
    "src/Panels/PanelGroup.cpp"
    "src/Panels/PanelGroup.h"
    "src/Panels/PanelSettings.cpp"
    "src/Panels/PanelSettings.h")

set(src_files
    "src/Editor.h"
    "src/Editor.cpp"
    "src/main.cpp")

set(all_files
    ${src_files}
    ${panel_files})
    
add_executable(${target_name} ${all_files})
add_executable(${PROJECT_NAME}::${target_name} ALIAS ${target_name})

target_include_directories(${target_name} PUBLIC "src")
target_link_libraries(${target_name}
    compiler_features
    compiler_warnings
    Wraith::Engine
    imgui
    ImGuizmo)

target_include_directories(${target_name} PUBLIC "src")

auto_group_sources(all_files ${CMAKE_CURRENT_SOURCE_DIR})
source_group("src/Precompiled" REGULAR_EXPRESSION "cmake_pch\.[ch]xx")

set_target_properties(${target_name} PROPERTIES
    FOLDER ${${PROJECT_NAME}_FOLDER}
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)