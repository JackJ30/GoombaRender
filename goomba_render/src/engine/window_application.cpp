#include "window_application.h"

#include "renderer/glad_context_creator.h"
#include "engine/window/sdl_window.h"

namespace GoombaEngine
{
    #define BIND_EVENT_FN(x) std::bind(&WindowApplication::x, this, std::placeholders::_1)

    void WindowApplication::Run()
    {
        if (m_Running) return;
        m_Running = true;

        //m_Window = std::make_unique<GoombaEngine::SDLWindow>(GoombaEngine::WindowProps(), GoombaRender::ConfigureSDLOpenGLContext, GoombaRender::CreateSDLOpenGLContext);
        OnInit();

        while (m_Running)
        {
            m_Window->Update();
            OnUpdate();
        }

        OnFinish();
    }
}