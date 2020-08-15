set(target_name "app")

set(src_files
    "src/main.cpp")

add_executable(${target_name} WIN32 ${src_files})

target_link_libraries(${target_name} engine-lib imgui)

set_target_properties(${target_name} PROPERTIES FOLDER ${PROJECT_NAME})