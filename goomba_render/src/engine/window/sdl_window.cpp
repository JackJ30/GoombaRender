#include "sdl_window.h"

namespace GoombaEngine
{
    static size_t s_Count = 0;

    SDLWindow::SDLWindow(WindowProperties properties) : m_Properties{properties}
    {
        GLogInfo("Creating SDL3 window...");

        // If there are no existing windows, initialize SDL
        if (s_Count == 0)
        {
            int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
            if (result < 0)
            {
                GLogCritical("SDL failed to initialize: {}", SDL_GetError());
            }
            GLogTrace("SDL initialized because there were no other SDL windows");

            // Configure SDL for creating opengl context
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
            GLogTrace("Configured SDL OpenGL Context");
        }

        m_Handle = SDL_CreateWindow(m_Properties.Title.c_str(), m_Properties.Width, m_Properties.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (m_Handle == NULL)
        {
            GLogCritical("SDL failed to create window: {}", SDL_GetError());
        }
        s_Count++;
        GLogTrace("SDL window created");

        // Have SDL create OpenGL context
        m_Context = SDL_GL_CreateContext(m_Handle);
        if (m_Context == NULL)
        {
            GLogCritical("SDL failed to create sdl-ogl context: {}", SDL_GetError());
        }
        GLogTrace("OpenGL SDL context created");

        // Load GLAD functions pointers for the context
        m_GraphicsContext.LoadContext(SDL_GL_GetProcAddress);

        SetVSync(m_Properties.VSync);

        GLogInfo("SDL3 window created");
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
    void SDLWindow::PollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (m_EventCallback) m_EventCallback(event);
        }
    }

    void SDLWindow::SwapBuffers()
    {
        int result = SDL_GL_SwapWindow(m_Handle);
        ASSERT(result == 0, "SDL failed to swap the window buffers: {}", SDL_GetError());
    }

    SDL_WindowID SDLWindow::GetSDLWindowID()
    {
        return SDL_GetWindowID(m_Handle);
    }

    void SDLWindow::SetVSync(bool enabled)
    {
        int result = SDL_GL_SetSwapInterval(enabled ? 1 : 0);
        ASSERT(result == 0, "SDL failed to set swap interval: {}", SDL_GetError());
        m_Properties.VSync = enabled;
    }

    void SDLWindow::RegisterEventCallback(std::function<void(SDL_Event&)> eventCallback)
    {
        m_EventCallback = eventCallback;
    }
}