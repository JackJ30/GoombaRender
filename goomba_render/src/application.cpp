#include "application.h"

namespace GoombaRender
{
    void Application::Start()
    {
        if (m_Running) return;
        m_Running = true;

        Init();

        while (m_Running)
        {
            Update();
        }

        Destroy();
    }
}