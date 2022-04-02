#include "Texture.h"

#include <stb/stb_image.h>

#include "DXUtil.h"
#include "Graphics/Framework.h"

namespace Wraith
{
    struct Texture::Data
    {
        std::string path = "";
        // ComPtr<ID3D11DepthStencilView> depth;
        // ComPtr<ID3D11RenderTargetView> render_target;
        // ComPtr<ID3D11ShaderResourceView> shader_resource;
        // ComPtr<ID3D11Texture2D> texture;
        Vec2u size = Vec2u();
        // D3D11_VIEWPORT viewport = {};
        // DXGI_FORMAT format;
        bool is_depth = false;
        bool is_rt = true;
    };

    Texture::Texture() { m_Data = std::make_unique<Data>(); }

    Texture::Texture(const std::string& path) { LoadFromFile(path); }

    Texture::Texture(const Vec2u& size, ImageFormat format, void* data) { Create(size, format, data); }

    Texture::Texture(const TextureCreateInfo& info) { Create(info); }

    Texture::Texture(const Texture& other) { operator=(other); }

    Texture::Texture(Texture&& other) { operator=(std::forward<Texture>(other)); }

    Texture& Texture::operator=(const Texture& other)
    {
        if (m_Data)
        {
            Release();
        }

        if (!other.m_Data->path.empty())
            LoadFromFile(other.m_Data->path);
        else
            ASSERT_LOG(false, "Copying Textures not implemented!");

        return *this;
    }

    Texture& Texture::operator=(Texture&& other)
    {
        if (m_Data)
        {
            Release();
        }

        m_Data = std::move(other.m_Data);
        return *this;
    }

    Texture::~Texture() { Release(); }

    bool Texture::IsValid() { return !m_Data /*&& (m_Data->shader_resource || m_Data->texture || m_Data->depth || m_Data->render_target)*/; }

    bool Texture::LoadFromFile(const std::string& path)
    {
        if (path.empty())
            return false;

        if (!std::filesystem::exists(std::filesystem::path(path)))
        {
            ERROR_LOG("Texture file '{}' not found", path.c_str());
            return false;
        }

        if (!m_Data)
            m_Data = std::make_unique<Data>();

        m_Data->path = path;

        i32 w, h, channels;
        unsigned char* image = stbi_load(m_Data->path.c_str(), &w, &h, &channels, STBI_rgb_alpha);
        if (image != nullptr)
        {
            Create({ (u32)w, (u32)h }, ImageFormat::R8G8B8A8_UNORM, image);
        }
        else
        {
            ERROR_LOG("Failed to load texture '{}'", m_Data->path.c_str());
            return false;
        }
        stbi_image_free(image);
        return true;
    }

    void Texture::Create(const Vec2u& size, ImageFormat format, void* data)
    {
        TextureCreateInfo info;
        info.size = size;
        info.format = format;
        info.data = data;
        Create(info);
    }

    void Texture::Create(const TextureCreateInfo& info)
    {
        if (!m_Data)
            m_Data = std::make_unique<Data>();

        m_Data->is_rt = info.render_target;
        // m_Data->format = (DXGI_FORMAT)info.format;

        // D3D11_TEXTURE2D_DESC desc = {};
        // desc.Width = (u32)info.size.x;
        // desc.Height = (u32)info.size.y;
        // desc.MipLevels = info.num_mips;
        // desc.ArraySize = 1;
        // desc.Format = m_Data->format;
        // desc.SampleDesc.Count = 1;
        // desc.SampleDesc.Quality = 0;
        // desc.Usage = static_cast<D3D11_USAGE>(info.usage);

        // if (m_Data->is_rt)
        //    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        // else
        //    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        // desc.CPUAccessFlags = info.cpu_access;
        // if (info.num_mips != 1)
        //    desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

        // D3D11_SUBRESOURCE_DATA initial_data = {};
        // initial_data.pSysMem = info.data;
        // initial_data.SysMemPitch = info.size.x * (sizeof(u8)) * 4;

        // ComPtr<ID3D11Texture2D> texture;
        // if (FailedCheck("Creating texture",
        //                Framework::GetDevice().CreateTexture2D(&desc, info.data ? &initial_data : nullptr, &texture)))
        //{
        //    Release();
        //    return;
        //}

        // CreateFromTexture(texture.Get());
        m_Data->size = info.size;
    }

    void Texture::CreateFromTexture(ID3D11Texture2D* texture)
    {
        if (!texture)
        {
            WARNING_LOG("CreateFromTexture called with a nullptr, this shouldn't happen");
            return;
        }

        if (m_Data->is_rt)
        {
            // if (FailedCheck("Creating Render Target View for texture",
            //                Framework::GetDevice().CreateRenderTargetView(texture, nullptr, &m_Data->render_target)))
            //{
            //    // cleanup.
            //    Release();
            //    return;
            //}
        }

        if (texture)
        {
            D3D11_TEXTURE2D_DESC desc;
            texture->GetDesc(&desc);
            // m_Data->viewport = { 0.0f, 0.0f, (f32)desc.Width, (f32)desc.Height, 0.0f, 1.0f };
            m_Data->size = { (u32)desc.Width, (u32)desc.Height };
        }
        // m_Data->texture = texture;
        // if (FailedCheck("Creating Shader Resource View for texture",
        //                Framework::GetDevice().CreateShaderResourceView(texture, nullptr, &m_Data->shader_resource)))
        //{
        //    // cleanup.
        //    Release();
        //    return;
        //}
    }

    void Texture::CreateDepth(const Vec2u& size, ImageFormat format)
    {
        // D3D11_TEXTURE2D_DESC desc = {};
        // desc.Width = (u32)size.x;
        // desc.Height = (u32)size.y;
        // desc.MipLevels = 1;
        // desc.ArraySize = 1;
        // desc.Format = DXGI_FORMAT_R32_TYPELESS;
        //(DXGI_FORMAT) format;
        // desc.SampleDesc.Count = 1;
        // desc.SampleDesc.Quality = 0;
        // desc.Usage = D3D11_USAGE_DEFAULT;
        // desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        // desc.CPUAccessFlags = 0;
        // desc.MiscFlags = 0;

        // D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
        // dsv_desc.Flags = 0;
        // dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
        // dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        // dsv_desc.Texture2D.MipSlice = 0;

        // D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
        // sr_desc.Format = DXGI_FORMAT_R32_FLOAT;
        // sr_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        // sr_desc.Texture2D.MostDetailedMip = 0;
        // sr_desc.Texture2D.MipLevels = 1;

        // ComPtr<ID3D11Texture2D> texture;
        // if (FailedCheck("Creating Texture2D for depth texture",
        //                Framework::GetDevice().CreateTexture2D(&desc, nullptr, &texture)))
        //{
        //    return;
        //}

        // if (FailedCheck("Creating Depth Stencil View for depth texture",
        //                Framework::GetDevice().CreateDepthStencilView(texture.Get(), &dsv_desc, &m_Data->depth)))
        //{
        //    // cleanup.
        //    Release();
        //    return;
        //}

        // if (FailedCheck(
        //        "Creating Shader Resource View for depth texture",
        //        Framework::GetDevice().CreateShaderResourceView(texture.Get(), &sr_desc, &m_Data->shader_resource)))
        //{
        //    // cleanup.
        //    Release();
        //    return;
        //}

        // m_Data->is_depth = true;
        // m_Data->texture = texture;
        // m_Data->viewport = { 0.0f, 0.0f, (f32)size.x, (f32)size.y, 0.0f, 1.0f };
        // m_Data->size = size;
    }

    void Texture::Clear(const Vec4f& color)
    {
        // Framework::GetContext().ClearRenderTargetView(m_Data->render_target.Get(), &color.x);
    }

    void Texture::ClearDepth(f32 depth, u32 stencil)
    {
        // Framework::GetContext().ClearDepthStencilView(
        //    m_Data->depth.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, (UINT8)stencil);
    }

    void Texture::Resize(const Vec2u& size, ImageFormat format, void* data)
    {
        bool depth = m_Data->is_depth;
        Release();
        if (depth)
        {
            CreateDepth(size, format);
        }
        else
        {
            Create(size, format, data);
        }
    }

    void Texture::UnsetActiveTarget()
    { /*Framework::GetContext().OMSetRenderTargets(0, nullptr, nullptr);*/
    }

    void Texture::SetAsActiveTarget(Texture* depth)
    {
        // Framework::GetContext().OMSetRenderTargets(
        //    1, m_Data->render_target.GetAddressOf(), depth ? depth->m_Data->depth.Get() : nullptr);
        // SetViewport();
    }

    void Texture::SetViewport()
    { /*Framework::GetContext().RSSetViewports(1, &m_Data->viewport);*/
    }

    void Texture::SetCustomViewport(f32 top_left_x, f32 top_left_y, f32 width, f32 height, f32 min_depth, f32 max_depth)
    {
        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = top_left_x;
        viewport.TopLeftY = top_left_y;
        viewport.Width = width;
        viewport.Height = height;
        viewport.MinDepth = min_depth;
        viewport.MaxDepth = max_depth;

        // Framework::GetContext().RSSetViewports(1, &viewport);
    }

    void Texture::Bind(u32 slot) const
    {
        // Framework::GetContext().PSSetShaderResources(slot, 1, m_Data->shader_resource.GetAddressOf());
    }

    void Texture::Unbind(u32 slot) const
    { /*Framework::GetContext().PSGetShaderResources(slot, 1, NULL);*/
    }

    void Texture::Release()
    {
        if (m_Data)
        {
            // m_Data->depth.Reset();
            // m_Data->render_target.Reset();
            // m_Data->shader_resource.Reset();
            // m_Data->texture.Reset();
        }
    }

    // ID3D11Texture2D* Texture::GetTexture() const { return m_Data->texture.Get(); }

    // ID3D11DepthStencilView* Texture::GetDepth() const { return m_Data->depth.Get(); }

    // ID3D11RenderTargetView* Texture::GetRenderTarget() const { return m_Data->render_target.Get(); }

    // ID3D11ShaderResourceView* Texture::GetShaderResourceView() const { return m_Data->shader_resource.Get(); }

    Vec2u Texture::GetSize() const { return m_Data->size; }

    Vec2f Texture::GetSizef() const { return { (f32)m_Data->size.x, (f32)m_Data->size.y }; }

    void Texture::Blit(const u8* data, i32 x, i32 y, i32 w, i32 h, i32 stride)
    {
        if (!m_Data)
        {
            ERROR_LOG("Texture not Initialized!");
            return;
        }

        // CD3D11_BOX box(x, y, 0, x + w, y + h, 1);
        // Framework::GetContext().UpdateSubresource(m_Data->texture.Get(), 0, &box, data, stride, 0);
    }
}  // namespace Wraith
