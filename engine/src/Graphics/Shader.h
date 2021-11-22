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
        Shader(std::underlying_type_t<ShaderType> shader_type, const std::string& path);
        Shader(const Shader& other);
        ~Shader();

        Shader& operator=(const Shader& other);

        bool Load(std::underlying_type_t<ShaderType> shader_type, const std::string& path);

        void Bind();
        void Unbind();

        ShaderType GetType() const;

        std::string GetName() const;

    private:
        bool CompileShader();

        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}  // namespace Wraith