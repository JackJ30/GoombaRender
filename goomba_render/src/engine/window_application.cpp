#include "window_application.h"

#include "renderer/glad_context_creator.h"
#include "engine/window/gl_framework_window.h"

namespace GoombaEngine
{
    #define BIND_EVENT_FN(x) std::bind(&WindowApplication::x, this, std::placeholders::_1)

    void WindowApplication::Run()
    {
        if (m_Running) return;
        m_Running = true;

        m_Window = std::make_unique<GoombaEngine::GLFrameworkWindow>(GoombaEngine::WindowProps(), GoombaRender::ConfigureGLFWOpenGLContext, GoombaRender::CreateGLFWOpenGLContext);
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        Init();

        while (m_Running)
        {
            m_Window->Update();

            Update();

            m_Window->SwapBuffers();
        }

        Finish();
    }

    void WindowApplication::OnEvent(Event &event)
    {
        EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    }

    bool WindowApplication::OnWindowClose(WindowCloseEvent& event)
	{
		Stop();
		return true;
	}
}