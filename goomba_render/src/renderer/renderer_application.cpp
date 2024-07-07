#include "renderer_application.h"
#include "engine/imgui_layer.h"

#include <imgui.h>

#include "renderer/vertex_array.h"
#include "renderer/shader.h"

namespace GoombaRender
{
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<Shader> shader;
    
    RendererApplication::RendererApplication()
    {
    
    }
    
    void RendererApplication::OnInit()
    {
        m_Window = std::make_unique<GoombaEngine::SDLWindow>(GoombaEngine::WindowProperties());
        m_Window->RegisterEventCallback(std::bind(&RendererApplication::OnEvent, this, std::placeholders::_1));
        GoombaEngine::ImGUISetup(*m_Window);
        
        GoombaEngine::GraphicsContext& context = m_Window->GetGraphicsContext();
        
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
        
        vertexBuffer = std::make_shared<VertexBuffer>(context, vertices, sizeof(vertices));
        vertexBuffer->SetLayout(layout);
        indexBuffer = std::make_shared<IndexBuffer>(context, indices, 3);
        
        vertexArray = std::make_shared<VertexArray>(context);
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);
        
        shader = std::make_shared<Shader>(context, "resources/shaders/test.glsl");
    }

    void RendererApplication::OnUpdate()
    {
        m_Window->PollEvents();
        GoombaEngine::ImGUIStartFrame();

        GoombaEngine::GraphicsContext& context = m_Window->GetGraphicsContext();
        
        {
            context.GetGlad().ClearColor(.1f, .2f, .3f, 1.0f);
            context.GetGlad().Clear(GL_COLOR_BUFFER_BIT);
            
            shader->Bind();
            vertexArray->Bind();
            
            context.GetGlad().DrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
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