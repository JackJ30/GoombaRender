#ifndef GOOMBARENDER_RENDERER_APPLICATION_H
#define GOOMBARENDER_RENDERER_APPLICATION_H


#include "engine/application.h"
#include "engine/window/sdl_window.h"
#include "engine/graphics_context.h"

namespace GoombaRender
{
    class RendererApplication : public GoombaEngine::Application
    {
    public:
        RendererApplication();
        
        void OnInit() override;
        void OnUpdate() override;
        void OnFinish() override;

    protected:
        std::unique_ptr<GoombaEngine::SDLWindow> m_Window;

    private:
        GoombaEngine::GraphicsContext m_Context;
        
        void OnEvent(SDL_Event& event);
    };
}

#endif // RENDERER_APPLICATION_H