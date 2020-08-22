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

		for (size_t i = 0; i < 50000; i++)
		{
			auto e = CreateEntity("Sprite_" + std::to_string(i+1));
			auto& sprite = e.AddComponent<SpriteComponent>();
			sprite.texture = i % 2 == 0 ? TextureID("assets/textures/test.jpg") : TextureID("assets/textures/default.png");
			sprite.origin = { 0.5f, 0.5f };
			sprite.color = { Rand(), Rand(), Rand(), 1 };
			sprite.layer = i;
			sprite.world_space = true;

			auto& transform = e.GetComponent<TransformComponent>();
			transform.position = { RandomRange(-m_Window->GetSizef().x, m_Window->GetSizef().x) * 32.0f, RandomRange(-m_Window->GetSizef().y, m_Window->GetSizef().y) * 32.0f };
			transform.rotation = RandomRange(0.0f, 360.0f);
			transform.scale = Vec2f{ 1, 1 } * 0.2f;
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
			transform.rotation -= dt * 5.0f;
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