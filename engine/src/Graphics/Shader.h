#pragma once

namespace Wraith
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
        Shader(Shader&& other);
        ~Shader();

        Shader& operator=(Shader&& other);

        bool Load(std::underlying_type_t<ShaderType> shader_type, const std::string& path);

        void Bind();
        void Unbind();

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}  // namespace Wraith