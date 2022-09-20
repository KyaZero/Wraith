set(target_name "Engine")

set(precompiled_files
  "src/Precompiled/Precompiled.cpp"
  "src/Precompiled/Precompiled.h")

set(threading_files
  "src/Threading/ThreadScheduler.h"
  "src/Threading/ThreadScheduler.cpp"
  "src/Threading/ProcessingThread.h"
  "src/Threading/ProcessingThread.cpp"
  "src/Threading/ProcessingJob.h"
  "src/Threading/ProcessingJob.cpp"
  "src/Threading/WorkerThread.h"
  "src/Threading/WorkerThread.cpp"
  "src/Threading/PersistentJob.h"
  "src/Threading/PersistentJob.cpp"
  "src/Threading/Thread.h"
  "src/Threading/Thread.cpp")

set(common_files
  "src/Core/Common.h"
  "src/Core/Constants.h"
  "src/Core/Filewatcher.cpp"
  "src/Core/Filewatcher.h"
  "src/Core/Logger.cpp"
  "src/Core/Logger.h"
  "src/Core/Singleton.h"
  "src/Core/StringID.h"
  "src/Core/Timer.h"
  "src/Core/Types.h")

set(graphics_files
  "src/Graphics/Text/Font.cpp"
  "src/Graphics/Text/Font.h"
  "src/Graphics/Text/FontManager.cpp"
  "src/Graphics/Text/FontManager.h"
  "src/Graphics/Text/Text.h"
  "src/Graphics/Models/ModelManager.h"
  "src/Graphics/Models/ModelManager.cpp"
  "src/Graphics/Models/StaticMesh.h"
  "src/Graphics/Models/StaticMesh.cpp"
  "src/Graphics/Models/Model.h"
  "src/Graphics/Models/Model.cpp"
  "src/Graphics/Models/Material.h"
  "src/Graphics/Models/Material.cpp"
  "src/Graphics/TestRenderSystem.h"
  "src/Graphics/TestRenderSystem.cpp"
  "src/Graphics/FrameInfo.h"
  "src/Graphics/BlendState.cpp"
  "src/Graphics/BlendState.h"
  "src/Graphics/ContentManager.h"
  "src/Graphics/ContentManager.cpp"
  "src/Graphics/TextureManager.h"
  "src/Graphics/TextureManager.cpp"
  "src/Graphics/OrthographicCameraController.h"
  "src/Graphics/OrthographicCameraController.cpp"
  "src/Graphics/OrthographicCamera.h"
  "src/Graphics/OrthographicCamera.cpp"
  "src/Graphics/Camera.h"
  "src/Graphics/Sampler.h"
  "src/Graphics/Sampler.cpp"
  "src/Graphics/RenderCommand.h"
  "src/Graphics/RenderSystem.h"
  "src/Graphics/BaseRenderer.h"
  "src/Graphics/SpriteRenderer.h"
  "src/Graphics/SpriteRenderer.cpp"
  "src/Graphics/ForwardRenderer.h"
  "src/Graphics/ForwardRenderer.cpp"
  "src/Graphics/RenderManager.h"
  "src/Graphics/RenderManager.cpp"
  "src/Graphics/ShaderReflection.h"
  "src/Graphics/ShaderReflection.cpp"
  "src/Graphics/Shader.h"
  "src/Graphics/Shader.cpp"
  "src/Graphics/VkShader.h"
  "src/Graphics/VkShader.cpp"
  "src/Graphics/VkUtil.h"
  "src/Graphics/Buffer.h"
  "src/Graphics/Buffer.cpp"
  "src/Graphics/DXUtil.h"
  "src/Graphics/ImageFormat.h"
  "src/Graphics/TextRenderer.h"
  "src/Graphics/TextRenderer.cpp"
  "src/Graphics/Texture.h"
  "src/Graphics/Texture.cpp"
  "src/Graphics/VkFramework.h"
  "src/Graphics/VkFramework.cpp"
  "src/Graphics/Framework.h"
  "src/Graphics/Framework.cpp"
  "src/Graphics/VkPipeline.h"
  "src/Graphics/VkPipeline.cpp"
  "src/Graphics/VkDescriptors.h"
  "src/Graphics/VkDescriptors.cpp"
  "src/Graphics/VkDevice.h"
  "src/Graphics/VkDevice.cpp"
  "src/Graphics/VkModel.h"
  "src/Graphics/VkModel.cpp"
  "src/Graphics/VkBuffer.h"
  "src/Graphics/VkBuffer.cpp"
  "src/Graphics/VkSwapchain.cpp"
  "src/Graphics/VkSwapchain.h")

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
  "src/Scene/Archive.h"
  "src/Scene/Components.h"
  "src/Scene/Entity.h"
  "src/Scene/Scene.cpp"
  "src/Scene/Scene.h"
  "src/Scene/EditorCamera.h"
  "src/Scene/EditorCamera.cpp"
  "src/Scene/ScriptableEntity.h")
    
set(input_files
  "src/Input/Keys.h"
  "src/Input/Input.h"
  "src/Input/Input.cpp")

set(src_files
  "src/Application.cpp"
  "src/Application.h"
  "src/Engine.cpp"
  "src/Engine.h"
  "src/Wraith.h"
  "src/Version.h")

include("cmake/generate_version.cmake")

set(all_files
  ${precompiled_files}
  ${threading_files}
  ${ui_files}
  ${input_files}
  ${scene_files}
  ${graphics_files}
  ${window_files}
  ${common_files}
  ${math_files}
  ${src_files})

add_library(${target_name} STATIC ${all_files} ${SHADERS})
add_library(${PROJECT_NAME}::${target_name} ALIAS ${target_name})
compile_shaders(${target_name} ${PROJECT_SOURCE_DIR}/bin/assets/engine/shaders ${PROJECT_SOURCE_DIR}/bin/assets/engine/shaders)

target_link_libraries(${target_name} PUBLIC
    compiler_features
    compiler_warnings
    Vulkan::Vulkan
    $<$<CONFIG:Debug>:${SHADERC_LIBD}>
    $<$<CONFIG:RelWithDebInfo>:${SHADERC_LIB}>
    $<$<CONFIG:Release>:${SHADERC_LIB}>
    $<$<CONFIG:MinSizeRel>:${SHADERC_LIB}>
    imgui
    ImGuizmo
    EnTT
    stb
    glfw
    glm
    color-console
    dubu_serialize
    dubu::rect_pack
    fribidi
    harfbuzz
    tinyobjloader
    msdfgen::msdfgen
    msdfgen::msdfgen-ext
    tinyfd)

target_precompile_headers(${target_name} PUBLIC "src/Precompiled/Precompiled.h")
target_include_directories(${target_name} PUBLIC "src")

auto_group_sources(all_files ${CMAKE_CURRENT_SOURCE_DIR})
source_group("src/Precompiled" REGULAR_EXPRESSION "cmake_pch\.[ch]xx")

set_target_properties(${target_name} PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})

add_custom_command(
  TARGET ${target_name} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${PROJECT_SOURCE_DIR}/bin/
  ${CMAKE_SOURCE_DIR}/bin/
)