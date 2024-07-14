#ifndef GOOMBARENDER_RENDERER_APPLICATION_H
#define GOOMBARENDER_RENDERER_APPLICATION_H


#include "engine/application.h"
#include "engine/window.h"
#include "engine/graphics_context.h"
#include "engine/application_loop.h"

namespace GoombaRender
{
    class RendererApplication : public GoombaEngine::Application
    {
    public:
        RendererApplication();
        
        void Run() override;
    
    private:
        std::unique_ptr<GoombaEngine::Window> m_Window;
        GoombaEngine::ApplicationLoop m_Loop;
        GoombaEngine::GraphicsContext m_Context;
        
        void Tick(double delta);
        void Render(double delta, double interpolation);
        
        void OnEvent(SDL_Event& event);
    };
}

#endif // RENDERER_APPLICATION_H