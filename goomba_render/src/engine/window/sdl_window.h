#ifndef GOOMBARENDER_SDL_WINDOW_H
#define GOOMBARENDER_SDL_WINDOW_H

#include "window.h"
#include "engine/graphics_context.h"

#include <SDL3/SDL.h>

namespace GoombaEngine
{
    class SDLWindow : public Window
    {
    public:
        SDLWindow(WindowProperties properties);
        virtual ~SDLWindow();

        SDLWindow(const SDLWindow&) = delete;
        SDLWindow& operator=(const SDLWindow&) = delete;
        SDLWindow(SDLWindow&& other) = delete;
        SDLWindow& operator=(SDLWindow&& other) = delete;
        
        void PollEvents() override;
        void SwapBuffers() override;

		void SetVSync(bool enabled) override;
        void RegisterEventCallback(std::function<void(SDL_Event&)> eventCallback);

        inline SDL_Window* GetHandle() const { return m_Handle; }
        inline unsigned int GetWidth() const { return m_Properties.Width; }
		inline unsigned int GetHeight() const { return m_Properties.Height; }
        inline SDL_GLContext& GetSDLContext() { return m_Context; };
        inline GraphicsContext& GetGraphicsContext() { return m_GraphicsContext; };
        SDL_WindowID GetSDLWindowID();
		virtual bool IsVSyncEnabled() const { return m_Properties.VSync; }

    private:
        WindowProperties m_Properties;
        SDL_Window* m_Handle = nullptr;
        SDL_GLContext m_Context = nullptr;
        GraphicsContext m_GraphicsContext;
        std::function<void(SDL_Event&)> m_EventCallback;
    };
}

#endif // SDL_WINDOW_H