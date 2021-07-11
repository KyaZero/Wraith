#pragma once

#include "Graphics/ContentManager.h"

namespace fw
{
    using StringIDType = IDType;
    class StringID
    {
    public:
        StringID()
            : m_Hash(0)
        { }
        StringID(StringIDType id)
            : m_Hash(id)
        { }
        StringID(const std::string& path)
        {
            IDType id = ContentManager::Get()->Register(path);
            m_Hash = id;
        }

        operator StringIDType() const
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
        StringIDType m_Hash;
    };
}  // namespace fw

namespace std
{
    template <>
    struct hash<fw::StringID>
    {
        std::size_t operator()(const fw::StringID& id) const
        {
            return std::hash<fw::StringIDType>()(id);
        }
    };
}  // namespace std