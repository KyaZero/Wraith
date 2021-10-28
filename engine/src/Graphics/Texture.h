#pragma once

#include "ImageFormat.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace Wraith
{
    struct TextureCreateInfo
    {
        Vec2u size;
        ImageFormat format = ImageFormat::R8G8B8A8_UNORM;
        void* data = nullptr;
        bool render_target = true;
        u32 num_mips = 1u;
        u8 usage = 0u;
        i64 cpu_access = 0u;
    };

    class Texture
    {
    public:
        Texture();
        Texture(const std::string& path);
        Texture(const Vec2u& size, ImageFormat format = ImageFormat::R8G8B8A8_UNORM, void* data = nullptr);
        Texture(const TextureCreateInfo& info);
        Texture(const Texture& other);
        Texture(Texture&& other);

        Texture& operator=(const Texture& other);
        Texture& operator=(Texture&& other);
        ~Texture();

        bool IsValid();
        bool LoadFromFile(const std::string& path);

        void Create(const Vec2u& size, ImageFormat format = ImageFormat::R8G8B8A8_UNORM, void* data = nullptr);
        void Create(const TextureCreateInfo& info);
        void CreateFromTexture(ID3D11Texture2D* texture);
        void CreateDepth(const Vec2u& size, ImageFormat format = ImageFormat::D32_FLOAT);

        void Clear(const Vec4f& color = Vec4f());
        void ClearDepth(f32 depth = 1.0f, u32 stencil = 0);

        void Resize(const Vec2u& size, ImageFormat format = ImageFormat::R8G8B8A8_UNORM, void* data = nullptr);

        void UnsetActiveTarget();
        void SetAsActiveTarget(Texture* depth = nullptr);
        void SetViewport();
        void SetCustomViewport(
            f32 top_left_x, f32 top_left_y, f32 width, f32 height, f32 min_depth = 0.0f, f32 max_depth = 0.0f);

        void Bind(u32 slot) const;
        void Unbind(u32 slot) const;
        void Release();

        ID3D11Texture2D* GetTexture() const;
        ID3D11DepthStencilView* GetDepth() const;
        ID3D11RenderTargetView* GetRenderTarget() const;
        ID3D11ShaderResourceView* GetShaderResourceView() const;
        Vec2u GetSize() const;
        Vec2f GetSizef() const;

        void Blit(const u8* data, i32 x, i32 y, i32 w, i32 h, i32 stride);

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}  // namespace Wraith