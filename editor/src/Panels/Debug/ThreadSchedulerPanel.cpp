#include "ThreadSchedulerPanel.h"

#include <Threading/ThreadScheduler.h>

void Wraith::ThreadSchedulerPanel::OnUIRender()
{
    if (Begin())
        ThreadScheduler::Get()->RenderDebugInfo();
    End();
}
