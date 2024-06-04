#include "renderer_application.h"

namespace GoombaRenderer
{
    void RendererApplication::OnInit()
    {
        m_Window = std::make_unique<GoombaEngine::SDLWindow>(GoombaEngine::WindowProperties());
        m_Window->RegisterEventCallback(std::bind(&RendererApplication::OnEvent, this, std::placeholders::_1));
    }

    void RendererApplication::OnUpdate()
    {
        m_Window->PollEvents();
        m_Window->MakeContextCurrent();

        m_Window->GetGladContext().ClearColor(.1f, .2f, .3f, 1.0f);
        m_Window->GetGladContext().Clear(GL_COLOR_BUFFER_BIT);

        m_Window->SwapBuffers();
    }

    void RendererApplication::OnFinish()
    {
    }

    void RendererApplication::OnEvent(SDL_Event &event)
    {
        switch(event.type)
        {
            case SDL_EVENT_QUIT: 
            {
                Stop();
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) 
                {
                    Stop();
                    break;
                }
            }
        }
    }
}