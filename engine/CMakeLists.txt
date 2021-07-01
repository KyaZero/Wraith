set(target_name "Engine")

set(precompiled_files
  "src/Precompiled/Precompiled.cpp"
  "src/Precompiled/Precompiled.h")

set(common_files
  "src/Core/Common.h"
  "src/Core/Filewatcher.cpp"
  "src/Core/Filewatcher.h"
  "src/Core/Logger.cpp"
  "src/Core/Logger.h"
  "src/Core/Singleton.h"
  "src/Core/Timer.h"
  "src/Core/Types.h")

set(graphics_files
  "src/Graphics/ContentManager.h"
  "src/Graphics/ContentManager.cpp"
  "src/Graphics/TextureManager.h"
  "src/Graphics/TextureManager.cpp"
  "src/Graphics/TextureID.h"
  "src/Graphics/OrthographicCameraController.h"
  "src/Graphics/OrthographicCameraController.cpp"
  "src/Graphics/OrthographicCamera.h"
  "src/Graphics/OrthographicCamera.cpp"
  "src/Graphics/EditorCamera.h"
  "src/Graphics/EditorCamera.cpp"
  "src/Graphics/Camera.h"
  "src/Graphics/Sampler.h"
  "src/Graphics/Sampler.cpp"
  "src/Graphics/RenderCommand.h"
  "src/Graphics/SpriteRenderer.h"
  "src/Graphics/SpriteRenderer.cpp"
  "src/Graphics/RenderManager.h"
  "src/Graphics/RenderManager.cpp"
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
  "src/Window/Window.h"
  "src/Window/Window.cpp")

set(ui_files
  "src/UI/ImGuiLayer.h"
  "src/UI/ImGuiLayer.cpp")

set(scene_files
  "src/Scene/ScriptableEntity.h"
  "src/Scene/Components.h"
  "src/Scene/Entity.h"
  "src/Scene/Scene.h"
  "src/Scene/Scene.cpp")
    
set(input_files
  "src/Input/Keys.h"
  "src/Input/Input.h"
  "src/Input/Input.cpp")

set(src_files
  "src/Engine.h"
  "src/Engine.cpp"
  "src/Application.h"
  "src/Application.cpp")

set(all_files
  ${precompiled_files}
  ${ui_files}
  ${input_files}
  ${scene_files}
  ${graphics_files}
  ${window_files}
  ${common_files}
  ${math_files}
  ${src_files})

add_library(${target_name} STATIC ${all_files})

target_link_libraries(${target_name} PUBLIC
    compiler_features
    compiler_warnings
    d3d11
    dxgi
    d3dcompiler
    imgui
    EnTT
    stb
    glfw
    color-console)

target_precompile_headers(${target_name} PRIVATE "src/Precompiled/Precompiled.h")
target_include_directories(${target_name} PUBLIC "src")

source_group("src" FILES ${src_files})
source_group("src/Window" FILES ${window_files})
source_group("src/Graphics" FILES ${graphics_files})
source_group("src/Scene" FILES ${scene_files})
source_group("src/Input" FILES ${input_files})
source_group("src/UI" FILES ${ui_files})
source_group("src/Core" FILES ${common_files})
source_group("src/Core/Math" FILES ${math_files})
source_group("src/Precompiled" FILES ${precompiled_files})
source_group("src/Precompiled" REGULAR_EXPRESSION "cmake_pch\.[ch]xx")

set_target_properties(${target_name} PROPERTIES FOLDER ${PROJECT_NAME})
