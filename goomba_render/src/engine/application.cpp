#include "application.h"

namespace GoombaEngine
{
    void Application::Run()
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