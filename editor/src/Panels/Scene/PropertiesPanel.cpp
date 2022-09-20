#include "PropertiesPanel.h"

#include <Graphics/TextureManager.h>
#include <Scene/Components.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <tinyfiledialogs.h>

namespace Wraith
{
    void PropertiesPanel::OnUIRender()
    {
        if (Begin())
        {
            DrawComponents();
        }
        End();
    }

    void PropertiesPanel::DrawComponents()
    {
        if (!m_SelectedEntity)
            return;

        Entity entity = m_SelectedEntity;

        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().tag;
            ImGui::InputText("Tag", &tag);
        }

        if (entity.HasComponent<TransformComponent>())
        {
            if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& transform = entity.GetComponent<TransformComponent>();

                ImGui::DragFloat3("Position", &transform.position.x, 0.01f);
                ImGui::DragFloat3("Scale", &transform.scale.x, 0.001f, 0.001f, 10.0f);
                Vec3f rotation_in_degrees = Degrees(Vec3f{ transform.rotation.GetEulerAngles() });
                ImGui::DragFloat3("Rotation", &rotation_in_degrees.x, 0.1f);
                transform.rotation.FromEuler(Radians(rotation_in_degrees));
            }
        }

        if (entity.HasComponent<TextComponent>())
        {
            if (ImGui::CollapsingHeader("Text Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& text = entity.GetComponent<TextComponent>();
                ImGui::InputTextMultiline("Text", &text.text);
                if (ImGui::InputText("Font", &text.font))
                {
                    text.font_id = StringID(text.font);
                }

                ImGui::DragInt("Font Size", &text.font_size, 1, 1, 256);

                ImGui::Separator();

                ImGui::RadioButton("Left", &text.justification, 0);
                ImGui::SameLine();
                ImGui::RadioButton("Center", &text.justification, 1);
                ImGui::SameLine();
                ImGui::RadioButton("Right", &text.justification, 2);

                ImGui::RadioButton("Top", &text.alignment, 0);
                ImGui::SameLine();
                ImGui::RadioButton("Middle", &text.alignment, 1);
                ImGui::SameLine();
                ImGui::RadioButton("Bottom", &text.alignment, 2);

                ImGui::RadioButton("LTR", &text.direction, 0);
                ImGui::SameLine();
                ImGui::RadioButton("RTL", &text.direction, 1);

                ImGui::Separator();

                ImGui::ColorEdit4("Color", &text.color.x);
                ImGui::SliderFloat("Blend Mode", &text.blend_mode, 0.0f, 1.0f, "Additive - %.2f - Alpha Blend");
            }
        }

        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& camera = entity.GetComponent<CameraComponent>();
                ImGui::Checkbox("Primary Camera", &camera.primary);
            }
        }

        if (entity.HasComponent<SpriteComponent>())
        {
            if (ImGui::CollapsingHeader("Sprite Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& sprite = entity.GetComponent<SpriteComponent>();
                // auto& current_tex = TextureManager::Get()->GetTexture(sprite.texture);
                // if (ImGui::ImageButton(current_tex.GetShaderResourceView(), { 64, 64 }))
                //{
                //    const char* filter = "*.png";
                //    auto path = tinyfd_openFileDialog("Load Texture", "", 1, &filter, nullptr, 0);
                //    if (path)
                //    {
                //        sprite.texture = StringID(path);
                //    }
                //}

                ImGui::ColorEdit4("Color", &sprite.color.x);
                ImGui::SliderFloat2("Origin", &sprite.origin.x, 0.0f, 1.0f);
                ImGui::SliderFloat("Layer", &sprite.layer, 0.0f, 1.0f);
                ImGui::Checkbox("Screen Space", &sprite.screen_space);
            }
        }

        // if (entity.HasComponent<ModelComponent>())
        //{
        //    if (ImGui::CollapsingHeader("Model Component", ImGuiTreeNodeFlags_DefaultOpen))
        //    {
        //        auto& model = entity.GetComponent<ModelComponent>();

        //        ImGui::Text("Meshes: ");
        //        ImGui::Indent();
        //        i32 i = 0;
        //        for (auto& mesh : model.model_instance.GetMeshes())
        //        {
        //            ImGui::Text("Mesh %d", i);
        //            ImGui::Indent();
        //            ImGui::Text("Material", i);
        //            ImGui::Indent();
        //            {
        //                auto& material = mesh->GetMaterial();
        //                ImGui::Text("Shader: %s", material.GetShader().GetName().c_str());
        //                ImGui::SameLine();
        //                if (ImGui::Button(std::format("Change").c_str()))
        //                {
        //                    const char* filter = "*.hlsl";
        //                    auto path = tinyfd_openFileDialog("Select Shader", "", 1, &filter, nullptr, 0);
        //                    if (path)
        //                    {
        //                        Shader s(material.GetShader().GetType(), path);
        //                        material.SetShader(s);
        //                    }
        //                }

        //                ImGui::Text("Textures: ");
        //                ImGui::Indent();
        //                auto& textures = material.GetTextures();
        //                for (auto& tex : textures)
        //                {
        //                    i32 slot = (i32)tex.slot;
        //                    ImGui::Text("Slot");
        //                    ImGui::SameLine();
        //                    ImGui::SliderInt(std::format("{}", (u64)&tex).c_str(), &slot, 0, 32);
        //                    tex.slot = slot;
        //                    // if (ImGui::ImageButton(tex.texture->GetShaderResourceView(), { 64, 64 }))
        //                    {
        //                        // const char* filter = "*.png";
        //                        // auto path = tinyfd_openFileDialog("Select Image", "", 1, &filter, nullptr, 0);
        //                        // if (path)
        //                        //{
        //                        //    tex = { tex.slot, &TextureManager::Get()->GetTexture(StringID(path)) };
        //                        //}
        //                    }
        //                }

        //                if (ImGui::Button("Add Texture"))
        //                {
        //                    material.AddTexture(0, &TextureManager::Get()->GetDefaultTexture());
        //                }
        //            }
        //        }
        //    }
        //}
    }
}  // namespace Wraith