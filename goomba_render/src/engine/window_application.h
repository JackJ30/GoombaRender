#ifndef WINDOW_APPLICATION_H
#define WINDOW_APPLICATION_H

#include "goombapch.h"

#include "engine/application.h"
#include "engine/window/sdl_window.h"

namespace GoombaEngine
{
    class WindowApplication : public Application
    {
    public:
        void Run() override final;

    protected:
        std::unique_ptr<GoombaEngine::SDLWindow> m_Window;

    private:
        void OnEvent(SDL_Event& event);
    };
}

#endif // WINDOW_APPLICATION_H