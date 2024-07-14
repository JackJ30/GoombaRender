#include "renderer_application.h"
#include "engine/imgui_layer.h"

#include <imgui.h>

#include "engine/input.h"
#include "renderer/vertex_array.h"
#include "renderer/shader.h"
#include "renderer/perspective_camera.h"
#include "renderer/texture.h"

namespace GoombaRender
{
    GoombaEngine::Input input;
    
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    glm::mat4 transform;
    
    Shader shader;
    Texture2D texture;
    
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
        
        BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float2, "a_TexCoord" }
        };
        
        vertexBuffer = std::make_shared<VertexBuffer>();
        vertexBuffer->AssignContext(m_Context);
        vertexBuffer->Create(vertices, sizeof(vertices));
        vertexBuffer->SetLayout(layout);
        
        indexBuffer = std::make_shared<IndexBuffer>();
        indexBuffer->AssignContext(m_Context);
        indexBuffer->Create(indices, 6);
        
        vertexArray = std::make_shared<VertexArray>();
        vertexArray->AssignContext(m_Context);
        vertexArray->Create();
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);
        
        transform = glm::translate(glm::mat4(1.0f),{0.0f, 0.0f, 0.0f});
        
        shader.AssignContext(m_Context);
        shader.Create("resources/shaders/test.glsl");
        
        texture.AssignContext(m_Context);
        texture.Create("resources/images/goomba.png");
        
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
            
            shader.Bind();
            shader.SetUniformMat4("u_Transform", transform);
            shader.SetUniformMat4("u_View", camera.GetViewMatrix());
            shader.SetUniformMat4("u_Projection", camera.GetProjectionMatrix());
            vertexArray->Bind();
            
            m_Context.GetGlad().DrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        ImGui::Begin("Loop Debug");
        ImGui::Text("Frame time: %f seconds", m_Loop.GetFrameTime());
        ImGui::Text("Tick rate : %f seconds", m_Loop.GetTickRate());
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
                // Rotation input is event based, so we don't want to worry about delta, just use a random value
                camera.ProcessRotationInput({event.motion.xrel, -event.motion.yrel}, 0.01);
            }
        }
    }
    
    void RendererApplication::Tick(double delta)
    {
        glm::vec3 move_direction(input.IsKeyPressed(SDLK_d) - input.IsKeyPressed(SDLK_a), input.IsKeyPressed(SDLK_e) - input.IsKeyPressed(SDLK_q), input.IsKeyPressed(SDLK_w) - input.IsKeyPressed(SDLK_s));
        camera.ProcessMovementInput(move_direction, delta);
    }
}