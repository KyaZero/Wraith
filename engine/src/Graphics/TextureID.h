#pragma once
#include <filesystem>
#include <string>

#include "ContentManager.h"
#include "Core/Types.h"

namespace fw
{
	using TextureIDType = IDType;
	class TextureID
	{
	public:
		TextureID()
		    : m_Hash(0)
		{ }
		TextureID(TextureIDType id)
		    : m_Hash(id)
		{ }
		TextureID(const std::string& path)
		{
			IDType id = ContentManager::Get()->Register(path);
			m_Hash = id;
		}

		operator TextureIDType() const
		{
			return m_Hash;
		}

	private:
		TextureIDType m_Hash;
	};
}  // namespace fw