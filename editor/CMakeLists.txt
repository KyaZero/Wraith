set(target_name "Editor")

set(panel_files
    "src/Panels/PropertiesPanel.h"
    "src/Panels/PropertiesPanel.cpp"
    "src/Panels/SceneHierarchyPanel.h"
    "src/Panels/SceneHierarchyPanel.cpp")

set(src_files
    "src/Editor.h"
    "src/Editor.cpp"
    "src/main.cpp")

set(all_files
    ${src_files}
    ${panel_files})
    
add_executable(${target_name} ${all_files})

target_link_libraries(${target_name}
    Engine
    compiler_features
    compiler_warnings
    imgui)

auto_group_sources(all_files ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${target_name} PROPERTIES
    FOLDER ${PROJECT_NAME}
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)