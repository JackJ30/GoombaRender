#include "application.h"

namespace GoombaEngine
{
    void Application::Run()
    {
        if (m_Running) return;
        m_Running = true;

        OnInit();

        while (m_Running)
        {
            OnUpdate();
        }

        OnFinish();
    }
}