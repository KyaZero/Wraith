#include "ForwardRenderer.h"

Wraith::ForwardRenderer::ForwardRenderer() { }

Wraith::ForwardRenderer::~ForwardRenderer() { }

bool Wraith::ForwardRenderer::Init()
{
    m_FrameBuffer.Init(sizeof(FrameBuffer), BufferUsage::Dynamic, BufferType::Constant, 0, &m_FrameBufferData);
    m_ObjectBuffer.Init(sizeof(ObjectBuffer), BufferUsage::Dynamic, BufferType::Constant, 0, &m_ObjectBufferData);

    // TODO: Perhaps move this to the Material, to allow for per-material sampling.
    m_Sampler.Init(Sampler::Filter::Linear, Sampler::Address::Clamp);

    return true;
}

void Wraith::ForwardRenderer::SetCamera(const SetCameraCommand& command)
{
    m_CurrentCamera = { command.proj, command.view };
}

void Wraith::ForwardRenderer::Render()
{
    auto& commands = GetCurrentCommands();
    // auto& context = Framework::GetContext();

    m_FrameBufferData.projection = m_CurrentCamera.projection;
    m_FrameBufferData.view = m_CurrentCamera.view;
    m_FrameBuffer.SetData(m_FrameBufferData);
    m_FrameBuffer.Bind();

    m_Sampler.Bind(0);

    for (auto& models : commands)
    {
        m_ObjectBufferData.transform = models.transform;
        m_ObjectBuffer.SetData(m_ObjectBufferData);
        m_ObjectBuffer.Bind(1);

        for (auto& mesh : models.model.GetMeshes())
        {
            if (!mesh->IsValid())
                continue;

            mesh->Bind();
            // context.DrawIndexed(mesh->GetNumIndices(), 0, 0);
        }
    }
}
