#pragma once

#include <entt/entt.hpp>

namespace dubu::serialize::internal
{
    template <>
    struct Serializer<entt::entity, false>
    {
        void Read(dubu::serialize::ReadBuffer& buffer, entt::entity& entity)
        {
            std::underlying_type_t<entt::entity> id;
            buffer >> id;
            entity = entt::entity{ id };
        }
        void Write(dubu::serialize::WriteBuffer& buffer, const entt::entity& entity) { buffer << std::underlying_type_t<entt::entity>(entity); }
    };
}  // namespace dubu::serialize::internal

namespace Wraith
{
    class OutputArchive
    {
    public:
        OutputArchive(dubu::serialize::WriteBuffer& buffer)
            : m_Buffer(buffer)
        { }

        void operator()(entt::entity entity) { m_Buffer << entity; }
        void operator()(std::underlying_type_t<entt::entity> entity) { m_Buffer << entity; }

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

        void operator()(entt::entity& entity) { m_Buffer >> entity; }
        void operator()(std::underlying_type_t<entt::entity>& entity) { m_Buffer >> entity; }

        template <typename T>
        void operator()(entt::entity& entity, T& component)
        {
            m_Buffer >> entity >> component;
        }

    private:
        dubu::serialize::ReadBuffer& m_Buffer;
    };

    template <typename... Components>
    class Archiver
    {
    public:
        void CreateSnapshot(entt::registry& registry)
        {
            OutputArchive outputArchive(m_Buffer);
            entt::snapshot{ registry }.entities(outputArchive).component<Components...>(outputArchive);
        }
        void RestoreSnapshot(entt::registry& registry)
        {
            registry.clear();

            InputArchive inputArchive(m_Buffer);
            entt::snapshot_loader{ registry }.entities(inputArchive).component<Components...>(inputArchive).orphans();

            m_Buffer = dubu::serialize::MemoryBuffer{};
        }

    private:
        dubu::serialize::MemoryBuffer m_Buffer;
    };
}  // namespace Wraith