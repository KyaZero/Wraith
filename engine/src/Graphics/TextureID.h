#pragma once

#include "ContentManager.h"

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

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> m_Hash;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << m_Hash;
        }

    private:
        TextureIDType m_Hash;
    };
}  // namespace fw

namespace std
{
    template <>
    struct hash<fw::TextureID>
    {
        std::size_t operator()(const fw::TextureID& id) const
        {
            return std::hash<fw::TextureIDType>()(id);
        }
    };
}  // namespace std