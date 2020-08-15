set(target_name "engine-lib")

set(common_files
    "src/Core/Logger.h"
    "src/Core/Logger.cpp"
    "src/Core/Common.h"
    "src/Core/Types.h"
    "src/Core/Timer.h")

set(graphics_files
    "src/Graphics/ShaderReflection.h"
    "src/Graphics/ShaderReflection.cpp"
    "src/Graphics/Shader.h"
    "src/Graphics/Shader.cpp"
    "src/Graphics/Buffer.h"
    "src/Graphics/Buffer.cpp"
    "src/Graphics/DXUtil.h"
    "src/Graphics/ImageFormat.h"
    "src/Graphics/Texture.h"
    "src/Graphics/Texture.cpp"
    "src/Graphics/Framework.h"
    "src/Graphics/Framework.cpp")

set(math_files
    "src/Core/Math/Vec.h"
    "src/Core/Math/Vec2.h"
    "src/Core/Math/Vec3.h"
    "src/Core/Math/Vec4.h"
    "src/Core/Math/Quat.h"
    "src/Core/Math/Mat4.h"
    "src/Core/Math/Mat3.h"
    "src/Core/Math/Mat.h")

set(window_files
    "src/Window/Mouse.h"
    "src/Window/Input.h"
    "src/Window/Input.cpp"
    "src/Window/Keys.h"
    "src/Window/WindowStyle.h"
    "src/Window/VideoMode.h"
    "src/Window/VideoMode.cpp"
    "src/Window/Event.h"
    "src/Window/Window.h"
    "src/Window/Window.cpp")

set(src_files
    "src/App.h"
    "src/App.cpp")

set(all_files
    ${graphics_files}
    ${window_files}
    ${common_files}
    ${math_files}
    ${src_files})

add_library(${target_name} STATIC ${all_files})

# set_target_properties(${target_name} PROPERTIES UNITY_BUILD ON)

target_precompile_headers(${target_name}
PRIVATE
  <string>
  <vector>
  <unordered_map>
  <memory>
  <algorithm>
  <functional>
  <queue>
  <cmath>
  <random>
  <filesystem>

  src/Core/Types.h
  src/Core/Logger.h
  src/Core/Math/Vec.h
  src/Core/Math/Mat.h
)

target_link_libraries(${target_name} PUBLIC d3d11 dxgi d3dcompiler imgui)
target_include_directories(${target_name} INTERFACE "src")

source_group("src" FILES ${src_files})
source_group("src\\Window" FILES ${window_files})
source_group("src\\Graphics" FILES ${graphics_files})
source_group("src\\Core" FILES ${common_files})
source_group("src\\Core\\Math" FILES ${math_files})

set_target_properties(${target_name} PROPERTIES FOLDER ${PROJECT_NAME})
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)