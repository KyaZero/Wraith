#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Graphics/TextureManager.h"
#include "imgui.h"

namespace fw
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	bool Scene::Init(Window* window, RenderManager* renderer)
	{
		m_Renderer = renderer;
		m_Window = window;
		m_Camera.Init(m_Window->GetSize().x, m_Window->GetSize().y);

		for (size_t y = 0; y < 100; ++y)
		{
			for (size_t x = 0; x < 100; ++x)
			{
				auto e = CreateEntity("Sprite_" + std::to_string(x+y + 1));
				auto& sprite = e.AddComponent<SpriteComponent>();
				sprite.texture = TextureID("assets/textures/test.jpg");
				sprite.origin = { 0.5f, 0.5f };
				sprite.color = { Rand(), Rand(), Rand(), 1 };
				sprite.layer = y;
				sprite.world_space = true;

				auto& transform = e.GetComponent<TransformComponent>();
				transform.position = { (f32)x * 1000, (f32)y * 1000 };
				//transform.rotation = RandomRange(0.0f, 360.0f);
				transform.scale = Vec2f{ 1, 1 } * 1.f;
			}
		}
		return true;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}

	void Scene::OnEvent(const Event& e)
	{
		m_Camera.OnEvent(e);
	}

	void Scene::Update(f32 dt, f32 total_time)
	{
		m_Camera.Update(dt);

		m_Renderer->Submit(SetCameraCommand{
			m_Camera.GetCamera()
			});

		ImGui::Begin("Entitites");
		auto v = m_Registry.view<TransformComponent, TagComponent>();
		for (auto& entity : v)
		{
			auto& [transform, tag] = v.get<TransformComponent, TagComponent>(entity);
			//transform.rotation -= dt * 5.0f;
			ImGui::Text(tag.tag.c_str());
		}
		ImGui::End();

		auto view = m_Registry.view<TransformComponent, SpriteComponent>();
		for (auto& entity : view)
		{
			auto& [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
			SpriteCommand sprite_cmd{
				sprite.texture,
				sprite.color,
				transform.position,
				sprite.origin,
				transform.scale,
				transform.rotation,
				sprite.layer,
				sprite.world_space
			};

			m_Renderer->Submit(sprite_cmd);
		}
	}
}