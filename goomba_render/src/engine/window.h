#ifndef GOOMBARENDER_WINDOW_H
#define GOOMBARENDER_WINDOW_H

#include "SDL3/SDL.h"
#include "glad/gl.h"

namespace GoombaEngine
{
    struct WindowProperties
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        bool VSync;
        
        WindowProperties(const std::string& title = "GoombaRender",
                         unsigned int width = 1280,
                         unsigned int height = 720,
                         bool vSync = true)
                : Title{title}, Width{width}, Height{height}, VSync{vSync}
        {
        }
    };
    
    class Window
    {
    public:
        Window(WindowProperties properties);
        virtual ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&& other) = delete;
        Window& operator=(Window&& other) = delete;
        
        void PollEvents();
        void SwapBuffers();
		void SetVSync(bool enabled);
        void RegisterEventCallback(std::function<void(SDL_Event&)> eventCallback);
        inline void SetMouseLockState(bool state) { SDL_SetRelativeMouseMode(state); };
        
        inline SDL_Window* GetHandle() const { return m_Handle; }
        inline unsigned int GetWidth() const { return m_Properties.Width; }
		inline unsigned int GetHeight() const { return m_Properties.Height; }
		inline bool IsVSyncEnabled() const { return m_Properties.VSync; }
        inline bool GetMouseLockState() { return SDL_GetRelativeMouseMode(); }
        inline SDL_GLContext& GetSDLContext() { return m_Context; };
        inline GLADloadfunc GetProcAddress() const { SDL_GL_MakeCurrent(m_Handle, m_Context); return SDL_GL_GetProcAddress; }
        inline SDL_WindowID GetSDLWindowID() const { return SDL_GetWindowID(m_Handle); }

    private:
        WindowProperties m_Properties;
        SDL_Window* m_Handle = nullptr;
        SDL_GLContext m_Context = nullptr;
        std::function<void(SDL_Event&)> m_EventCallback;
    };
}

#endif // SDL_WINDOW_H