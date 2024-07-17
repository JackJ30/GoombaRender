#include "renderer_application.h"
#include "engine/imgui_layer.h"

#include <imgui.h>

#include "renderer/vertex_array.h"
#include "renderer/shader.h"
#include "renderer/perspective_camera.h"
#include "renderer/texture.h"

// TODO - Window resizing should affect framebuffers... somehow

namespace GoombaRender
{
    PerspectiveCamera camera({0.0, 0.0, 1.0});
    
    RendererApplication::RendererApplication()
    {
    }
    
    void RendererApplication::Run()
    {
        m_Window = std::make_unique<GoombaEngine::Window>(GoombaEngine::WindowProperties());
        m_Window->RegisterEventCallback(std::bind(&RendererApplication::OnEvent, this, std::placeholders::_1));
        GoombaEngine::ImGUISetup(*m_Window);
        
        // Load GLAD functions pointers for the context
        m_Context.LoadContext(m_Window->GetProcAddress());
        
        // Setup Renderer
        m_Renderer = std::make_unique<Renderer>(m_Context);
        
        // Setup Loop
        m_Loop.RegisterRenderCallback(std::bind(&RendererApplication::Render, this, std::placeholders::_1, std::placeholders::_2));
        m_Loop.RegisterTickCallback(std::bind(&RendererApplication::Tick, this, std::placeholders::_1));
        
        camera.SetAspect(static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight()));
        
        float vertices[4 * 5] = {
                -1.0, -1.0, 0.0,  0.0f, 0.0f,
                1.0, -1.0, 0.0,   1.0f, 0.0f,
                1.0, 1.0, 0.0, 1.0f, 1.0f,
                -1.0, 1.0, 0.0,0.0f, 1.0f,
        };
        unsigned int indices[6] = {
                0, 1, 2,
                2, 3, 0
        };
        
        Scene testScene;
        testScene.m_ModelsToLoad.emplace_back("resources/models/testcube.gltf");
        
        m_Renderer->LoadScene(testScene);
        
        GLogInfo(m_Renderer->GetModel(testScene.m_Objects[0]).GetMeshCount());
        
        // LOOP
        m_Loop.Run();
        // LOOP
        
        GoombaEngine::ImGUIShutdown();
    }
    
    void RendererApplication::Render(double delta, double interpolation)
    {
        m_Window->PollEvents();
        GoombaEngine::ImGUIStartFrame();
        
        {
            m_Context.GetGlad().ClearColor(.1f, .2f, .3f, 1.0f);
            m_Context.GetGlad().Clear(GL_COLOR_BUFFER_BIT);
        }

        ImGui::Begin("Loop Debug");
        ImGui::Text("Frame time: %f seconds", m_Loop.GetFrameTime());
        ImGui::Text("Tick rate : %f seconds", m_Loop.GetTickRate());
        ImGui::End();

        GoombaEngine::ImGUIRender();
        m_Window->SwapBuffers();
    }
    
    void RendererApplication::Tick(double delta)
    {
        glm::vec3 move_direction(m_Input.IsKeyPressed(SDLK_d) - m_Input.IsKeyPressed(SDLK_a), m_Input.IsKeyPressed(SDLK_e) - m_Input.IsKeyPressed(SDLK_q), m_Input.IsKeyPressed(SDLK_w) - m_Input.IsKeyPressed(SDLK_s));
        camera.ProcessMovementInput(move_direction, delta);
    }

    void RendererApplication::OnEvent(SDL_Event &event)
    {
        GoombaEngine::ImGUIProcessEvent(&event);
        m_Input.ProcessEvent(event);

        switch(event.type)
        {
            case SDL_EVENT_QUIT:
            {
                m_Loop.Stop();
                break;
            }
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                if (event.window.windowID == m_Window->GetSDLWindowID()) m_Loop.Stop();
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            {
                int w = m_Window->GetWidth();
                int h = m_Window->GetHeight();
                m_Context.GetGlad().Viewport(0,0,w, h);
                camera.SetAspect(static_cast<float>(w) / static_cast<float>(h));
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    m_Window->SetMouseLockState(true);
                }
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        if (m_Window->GetMouseLockState()) m_Window->SetMouseLockState(false);
                        else m_Loop.Stop();
                        
                        break;
                    }
                }
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {
                // Rotation input is event based, so we don't want to worry about delta, just use a random value
                camera.ProcessRotationInput({event.motion.xrel, -event.motion.yrel}, 0.01);
                break;
            }
        }
    }
    
}