#pragma once

namespace Wraith
{
    class BlendState
    {
    public:
        BlendState();
        ~BlendState();

        bool Create(u8 src_rgb, u8 dest_rgb, u8 src_alpha, u8 dest_alpha, u8 op_rgb = 1u, u8 op_alpha = 1u);

        void Bind();
        void Unbind();

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };

}  // namespace Wraith