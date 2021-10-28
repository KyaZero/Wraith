#pragma once

namespace Wraith
{
    enum class BlendMode
    {
        Zero = 1,
        One = 2,
        SrcColor = 3,
        InvSrcColor = 4,
        SrcAlpha = 5,
        InvSrcAlpha = 6,
        DestAlpha = 7,
        InvDestAlpha = 8,
        DestColor = 9,
        InvDestColor = 10,
        SrcAlphaSat = 11,
        BlendFactor = 14,
        InvBlendFactor = 15,
        Src1Color = 16,
        InvSrc1Color = 17,
        Src1Alpha = 18,
        InvSrc1Alpha = 19
    };

    enum class BlendOp
    {
        Add = 1,
        Subtract = 2,
        RevSubtract = 3,
        Min = 4,
        Max = 5
    };

    class BlendState
    {
    public:
        BlendState();
        ~BlendState();

        bool Create(BlendMode src_rgb,
                    BlendMode dest_rgb,
                    BlendMode src_alpha,
                    BlendMode dest_alpha,
                    BlendOp op_rgb = BlendOp::Add,
                    BlendOp op_alpha = BlendOp::Add);

        void Bind();
        void Unbind();

    private:
        struct Data;
        std::unique_ptr<Data> m_Data;
    };

}  // namespace Wraith