#include "window_application.h"

#include "renderer/glad_context_creator.h"
#include "engine/window/gl_framework_window.h"

void GoombaEngine::WindowApplication::Run()
{
    if (m_Running) return;
    m_Running = true;

    m_Window = std::make_unique<GoombaEngine::GLFrameworkWindow>(GoombaEngine::WindowProps(), GoombaRender::ConfigureGLFWOpenGLContext, GoombaRender::CreateGLFWOpenGLContext);
    Init();

    while (m_Running)
    {
        m_Window->Update();

        Update();

        m_Window->SwapBuffers();
    }

    Finish();
}