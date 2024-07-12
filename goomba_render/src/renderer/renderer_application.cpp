#include "renderer_application.h"
#include "engine/imgui_layer.h"

#include <imgui.h>

#include "engine/input.h"
#include "renderer/vertex_array.h"
#include "renderer/shader.h"
#include "renderer/perspective_camera.h"

namespace GoombaRender
{
    GoombaEngine::Input input;
    
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<Shader> shader;
    glm::mat4 transform;
    
    PerspectiveCamera camera({0.0, 0.0, 1.0});
    
    RendererApplication::RendererApplication()
    {
    }
    
    void RendererApplication::Run()
    {
        m_Window = std::make_unique<GoombaEngine::SDLWindow>(GoombaEngine::WindowProperties());
        m_Window->RegisterEventCallback(std::bind(&RendererApplication::OnEvent, this, std::placeholders::_1));
        GoombaEngine::ImGUISetup(*m_Window);
        
        // Load GLAD functions pointers for the context
        m_Context.LoadContext(m_Window->GetProcAddress());
        
        // Setup Loop
        m_Loop.RegisterRenderCallback(std::bind(&RendererApplication::Render, this, std::placeholders::_1, std::placeholders::_2));
        m_Loop.RegisterTickCallback(std::bind(&RendererApplication::Tick, this, std::placeholders::_1));
        
        camera.SetAspect(static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight()));
        
        float vertices[3 * 7] = {
                -0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
                0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
                0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0
        };
        unsigned int indices[3] = {
                0, 1, 2
        };
        
        BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
        };
        
        vertexBuffer = std::make_shared<VertexBuffer>(m_Context, vertices, sizeof(vertices));
        vertexBuffer->SetLayout(layout);
        indexBuffer = std::make_shared<IndexBuffer>(m_Context, indices, 3);
        
        vertexArray = std::make_shared<VertexArray>(m_Context);
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);
        
        transform = glm::translate(glm::mat4(1.0f),{0.0f, 0.0f, 0.0f});
        
        shader = std::make_shared<Shader>(m_Context, "resources/shaders/test.glsl");
        
        // LOOP
        m_Loop.Run();
        // LOOP
        
        GoombaEngine::ImGUIShutdown();
    }

    void RendererApplication::Render(double delta, double interpolation)
    {
        m_Window->PollEvents();
        GoombaEngine::ImGUIStartFrame();
        
        glm::vec3 move_direction(input.IsKeyPressed(SDLK_d) - input.IsKeyPressed(SDLK_a), input.IsKeyPressed(SDLK_e) - input.IsKeyPressed(SDLK_q), input.IsKeyPressed(SDLK_w) - input.IsKeyPressed(SDLK_s));
        camera.ProcessMovementInput(move_direction);
        
        {
            m_Context.GetGlad().ClearColor(.1f, .2f, .3f, 1.0f);
            m_Context.GetGlad().Clear(GL_COLOR_BUFFER_BIT);
            
            shader->Bind();
            shader->SetUniformMat4("u_Transform", transform);
            shader->SetUniformMat4("u_View", camera.GetViewMatrix());
            shader->SetUniformMat4("u_Projection", camera.GetProjectionMatrix());
            vertexArray->Bind();
            
            m_Context.GetGlad().DrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::End();

        GoombaEngine::ImGUIRender();
        m_Window->SwapBuffers();
    }

    void RendererApplication::OnEvent(SDL_Event &event)
    {
        GoombaEngine::ImGUIProcessEvent(&event);
        input.ProcessEvent(event);

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
            case SDL_EVENT_KEY_DOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        m_Loop.Stop();
                        break;
                    }
                }
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {
                camera.ProcessRotationInput({event.motion.xrel, -event.motion.yrel});
            }
        }
    }
    
    void RendererApplication::Tick(double delta)
    {
    
    }
}