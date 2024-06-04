#include "sdl_window.h"

#include "renderer/glad_context_creator.h"

namespace GoombaEngine
{
    static size_t s_Count = 0;
    static SDLWindow* s_CurrentContextOwner;

    SDLWindow::SDLWindow(WindowProperties properties) : m_Properties{properties}
    {
        GLogTrace("Creating SDL3 window...");

        if (s_Count == 0)
        {
            int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
            if (result < 0)
            {
                GLogCritical("SDL failed to initialize: {}", SDL_GetError());
            }

            GoombaRender::ConfigureSDLOpenGLContext();
        }

        m_Handle = SDL_CreateWindow(m_Properties.Title.c_str(), m_Properties.Width, m_Properties.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (m_Handle == NULL)
        {
            GLogCritical("SDL failed to create window: {}", SDL_GetError());
        }
        ++s_Count;
        GLogInfo("SDL window created");

        m_Context = SDL_GL_CreateContext(m_Handle);
        if (m_Context == NULL)
        {
            GLogCritical("SDL failed to create context: {}", SDL_GetError());
        }
        GLogInfo("OpenGL SDL context created");

        MakeContextCurrent();
        GoombaRender::RetrieveSDLOpenGLFunctionPtrs(m_GladContext);
        GLogInfo("OpenGL SDL context setup");

        SetVSync(m_Properties.VSync);
    }

    SDLWindow::~SDLWindow()
    {
        if (m_Handle)
        {
            SDL_DestroyWindow(m_Handle);
            m_Handle = nullptr;
            --s_Count;
            GLogInfo("SDL window destroyed");

            if (s_Count == 0)
            {
                SDL_Quit();
                GLogInfo("SDL quit");
            }
        }
    }

    void SDLWindow::MakeContextCurrent()
    {
        int result = SDL_GL_MakeCurrent(m_Handle, m_Context);
        if (result < 0)
        {
            GLogError("SDL failed to make context current: {}", SDL_GetError());
            return;
        }

        if(s_CurrentContextOwner != nullptr) s_CurrentContextOwner->MakeContextNotCurrent();

        s_CurrentContextOwner = this;
        m_ContextCurrent = true;
    }

    void SDLWindow::PollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // Send the event somewhere
            /* switch(event.type)
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
            } */
        }
    }

    void SDLWindow::SwapBuffers()
    {
        DEBUG_ASSERT(m_ContextCurrent, "The window's context must be current before swapping buffers.");
        int result = SDL_GL_SwapWindow(m_Handle);
        if (result < 0)
        {
            GLogError("SDL failed to swap the window buffers: {}", SDL_GetError());
            return;
        }
    }

    void SDLWindow::SetVSync(bool enabled)
    {
        DEBUG_ASSERT(m_ContextCurrent, "The window's context must be current before setting vsync.");
        int result = SDL_GL_SetSwapInterval(enabled ? 1 : 0);
        if (result < 0)
        {
            GLogError("SDL failed to set swap interval: {}", SDL_GetError());
            return;
        }
        m_Properties.VSync = enabled;
    }

    void SDLWindow::MakeContextNotCurrent()
    {
        m_ContextCurrent = false;
        s_CurrentContextOwner = nullptr;
    }
}