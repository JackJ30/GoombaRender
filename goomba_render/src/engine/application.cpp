#include "application.h"

namespace GoombaEngine
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

        Finish();
    }
}