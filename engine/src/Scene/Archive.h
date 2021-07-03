#pragma once

#include <dubu_serialize/dubu_serialize.h>

#include "Entity.h"

namespace dubu::serialize::internal
{
    template <>
    struct Serializer<entt::entity, false>
    {
        void Read(dubu::serialize::ReadBuffer& buffer, entt::entity& entity)
        {
            std::underlying_type_t<entt::entity> id;
            buffer >> id;
            entity = static_cast<entt::entity>(id);
        }
        void Write(dubu::serialize::WriteBuffer& buffer, const entt::entity& entity)
        {
            buffer << static_cast<std::underlying_type_t<entt::entity>>(entity);
        }
    };
}  // namespace dubu::serialize::internal

namespace fw
{
    class OutputArchive
    {
    public:
        OutputArchive(dubu::serialize::WriteBuffer& buffer)
            : m_Buffer(buffer)
        { }

        void operator()(entt::entity entity)
        {
            m_Buffer << entity;
        }
        void operator()(std::underlying_type_t<entt::entity> entity)
        {
            m_Buffer << entity;
        }

        template <typename T>
        void operator()(entt::entity entity, const T& component)
        {
            m_Buffer << entity << component;
        }

    private:
        dubu::serialize::WriteBuffer& m_Buffer;
    };

    class InputArchive
    {
    public:
        InputArchive(dubu::serialize::ReadBuffer& buffer)
            : m_Buffer(buffer)
        { }

        void operator()(entt::entity& entity)
        {
            m_Buffer >> entity;
        }
        void operator()(std::underlying_type_t<entt::entity>& entity)
        {
            m_Buffer >> entity;
        }

        template <typename T>
        void operator()(entt::entity& entity, T& component)
        {
            m_Buffer >> entity >> component;
        }

    private:
        dubu::serialize::ReadBuffer& m_Buffer;
    };
}  // namespace fw