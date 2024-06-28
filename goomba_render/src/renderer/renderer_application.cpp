#include "renderer_application.h"
#include "engine/imgui_layer.h"

#include <imgui.h>

namespace GoombaRenderer
{
    void RendererApplication::OnInit()
    {
        m_Window = std::make_unique<GoombaEngine::SDLWindow>(GoombaEngine::WindowProperties());
        m_Window->RegisterEventCallback(std::bind(&RendererApplication::OnEvent, this, std::placeholders::_1));
        GoombaEngine::ImGUISetup(*m_Window);
    }

    void RendererApplication::OnUpdate()
    {
        m_Window->PollEvents();
        m_Window->MakeContextCurrent();
        GoombaEngine::ImGUIStartFrame();

        GoombaEngine::GraphicsContext& context = m_Window->GetGraphicsContext();
        
        {
            context.GetGlad().ClearColor(.1f, .2f, .3f, 1.0f);
            context.GetGlad().Clear(GL_COLOR_BUFFER_BIT);
            
            context.GetGlad().
        }

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        GoombaEngine::ImGUIRender();
        m_Window->SwapBuffers();
    }

    void RendererApplication::OnFinish()
    {
        GoombaEngine::ImGUIShutdown();
    }

    void RendererApplication::OnEvent(SDL_Event &event)
    {
        GoombaEngine::ImGUIProcessEvent(&event);

        switch(event.type)
        {
            case SDL_EVENT_QUIT: 
            {
                Stop();
                break;
            }
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                if (event.window.windowID == m_Window->GetSDLWindowID()) Stop();
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