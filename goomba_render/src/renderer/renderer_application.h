#ifndef GOOMBARENDER_RENDERER_APPLICATION_H
#define GOOMBARENDER_RENDERER_APPLICATION_H

#include "goombapch.h"

#include "engine/application.h"
#include "engine/window/sdl_window.h"

namespace GoombaRenderer
{
    class RendererApplication : public GoombaEngine::Application
    {
    public:
        void OnInit() override;
        void OnUpdate() override;
        void OnFinish() override;

    protected:
        std::unique_ptr<GoombaEngine::SDLWindow> m_Window;

    private:
        void OnEvent(SDL_Event& event);
    };
}

#endif // RENDERER_APPLICATION_H