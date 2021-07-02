#pragma once

namespace fw
{
    template <typename T>
    class Singleton
    {
    public:
        static T* Get()
        {
            return m_Instance.get();
        }
        template <typename... Args>
        static void Create(Args&&... args)
        {
            if (m_Instance)
            {
                throw std::runtime_error(
                    std::format("Trying to create a {} when it's already been created!", typeid(T).name()));
            }
            m_Instance = std::make_unique<T>(std::forward<Args>(args)...);
        }
        static void Destroy()
        {
            m_Instance.reset();
        }

    private:
        static inline std::unique_ptr<T> m_Instance;
    };
}  // namespace fw