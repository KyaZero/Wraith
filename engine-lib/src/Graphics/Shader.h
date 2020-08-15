#pragma once
#include <type_traits>

namespace fw
{
	class Shader
	{
	public:
		enum ShaderType : u8
		{
			Vertex = 1 << 0,
			Geometry = 1 << 1,
			Pixel = 1 << 2,
		};

		Shader();
		Shader(const Shader& other);
		Shader(Shader&& other);
		~Shader();

		Shader& operator=(const Shader& other);
		Shader& operator=(Shader&& other);

		bool Load(std::underlying_type_t<ShaderType> shader_type, const std::string& path);

		void Bind();
		void Unbind();

	private:
		struct Data;
		std::shared_ptr<Data> m_Data;
	};
}