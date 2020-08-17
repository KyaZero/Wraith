#pragma once
#include "Texture.h"
#include "TextureID.h"
#include <map>

namespace fw
{
	class TextureManager
	{
	public:
		static void Create();
		static void Destroy();
		static TextureManager* Get();

		const Texture& GetTexture(TextureID id);
		const Texture& GetDefaultTexture();

	private:
		void ValidateDefaultTexture();

		static TextureManager* s_Instance;

		Texture m_DefaultTexture;

		std::map<TextureID, Texture> m_TextureMap;
	};
}