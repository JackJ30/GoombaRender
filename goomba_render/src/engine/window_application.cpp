#include "window_application.h"

#include "engine/window/sdl_window.h"

namespace GoombaEngine
{
    #define BIND_EVENT_FN(x) std::bind(&WindowApplication::x, this, std::placeholders::_1)

    void WindowApplication::Run()
    {
        if (m_Running) return;
        m_Running = true;

        m_Window = std::make_unique<GoombaEngine::SDLWindow>(GoombaEngine::WindowProperties());
        m_Window->RegisterEventCallback(BIND_EVENT_FN(OnEvent));
        OnInit();

        while (m_Running)
        {
            m_Window->PollEvents();
            OnUpdate();
            m_Window->SwapBuffers();
        }

        OnFinish();
    }

    void WindowApplication::OnEvent(SDL_Event &event)
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