#include "application.h"

#include "graphics_context.h"
#include "engine/application_loop.h"
#include "engine/input.h"
#include "engine/imgui_layer.h"
#include "renderer/resource_manager.h"
#include "renderer/neorenderer.h"
#include "renderer/perspective_camera.h"

#include <glm/gtx/string_cast.hpp>

namespace GoombaRender
{
    std::unique_ptr<GoombaEngine::Window> window;
    GoombaEngine::ApplicationLoop loop;
    GoombaEngine::Input input;
    
    PerspectiveCamera camera{{0.0, 0.0, 1.0}};
    std::unique_ptr<VertexArrayInfo> array;
    std::shared_ptr<ShaderInfo> shader;
    
    void RunApplication()
    {
        GLogInfo("Application starting..");
        
        window = std::make_unique<GoombaEngine::Window>(GoombaEngine::WindowProperties{});
        window->RegisterEventCallback(&OnEvent);
        GoombaEngine::ImGUISetup(*window);
        
        // Load GLAD functions pointers for the context
        LoadGraphicsContext(window->GetProcAddress());
        
        // Setup Renderer
        //m_Renderer = std::make_unique<Renderer>(context);
        
        // Setup Loop
        loop.RegisterRenderCallback(&Render);
        loop.RegisterTickCallback(&Tick);
        
        camera.SetAspect(static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()));
        
        float vertices[4 * 5] = {
                -1.0, -1.0, 0.0,  0.0f, 0.0f,
                1.0, -1.0, 0.0,   1.0f, 0.0f,
                1.0, 1.0, 0.0, 1.0f, 1.0f,
                -1.0, 1.0, 0.0,0.0f, 1.0f,
        };
        BufferLayout layout({
            {ShaderDataType::Float3, "v_Position"},
            {ShaderDataType::Float2, "v_TexCoord"}
        });
        unsigned int indices[6] = {
                0, 1, 2,
                2, 3, 0
        };
        
        unsigned int vbo, ibo;
        glad.GenBuffers(1, &vbo);
        glad.BindBuffer(GL_ARRAY_BUFFER, vbo);
        glad.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glad.GenBuffers(1, &ibo);
        glad.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glad.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        array = std::unique_ptr<VertexArrayInfo>(CreateVertexArray());
        array->BindBufferLayout(vbo, layout);
        IndicesSection indicesSection = {0, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT};
        array->SetIndexBuffer(ibo, {indicesSection});
        //testScene.m_Objects.push_back({Asset<Model>("resources/models/AntiqueCamera.glb"), Transform()});
        //testScene.m_Objects.push_back({Asset<Model>("resources/models/testcube.gltf"), Transform({0.0, 3.0, 0.0})});
        //testScene.LoadAssets();
        
        shader = LoadShader("resources/shaders/test.glsl");
        
        // LOOP
        loop.Run();
        // LOOP
        
        GLogInfo("Application shutting down...");
        GoombaEngine::ImGUIShutdown();
    }
    
    void Tick(double delta)
    {
        window->PollEvents();
        GoombaEngine::ImGUIStartFrame();
        
        {
            glad.ClearColor(.1f, .2f, .3f, 1.0f);
            glad.Clear(GL_COLOR_BUFFER_BIT); // TODO - move screen clearing to renderer
            
            glad.UseProgram(shader->GetRendererID());
            glad.DrawElements(array->drawMode, array->indicesInfo[0].count, array->indicesInfo[0].type, (const void*)array->indicesInfo[0].offset);
            //m_Renderer->AddScenePass(camera, testScene);
            //m_Renderer->Render();
        }
        
        ImGui::Begin("Loop Debug");
        ImGui::Text("Frame time: %f seconds", loop.GetFrameTime());
        ImGui::Text("Tick rate : %f seconds", loop.GetTickRate());
        ImGui::End();
        
        ImGui::Begin("Camera Debug");
        ImGui::Text("Camera Forward: %s", glm::to_string(camera.GetForward()).c_str());
        ImGui::Text("Camera Position: %s", glm::to_string(camera.GetPosition()).c_str());
        ImGui::End();
        
        GoombaEngine::ImGUIRender();
        window->SwapBuffers();
    }
    
    void Render(double delta, double interpolation)
    {
        glm::vec3 move_direction(input.IsKeyPressed(SDLK_d) - input.IsKeyPressed(SDLK_a), input.IsKeyPressed(SDLK_e) - input.IsKeyPressed(SDLK_q), input.IsKeyPressed(SDLK_w) - input.IsKeyPressed(SDLK_s));
        camera.ProcessMovementInput(move_direction, delta);
    }
    
    void OnEvent(SDL_Event &event)
    {
        GoombaEngine::ImGUIProcessEvent(&event);
        input.ProcessEvent(event);
        
        switch(event.type)
        {
            case SDL_EVENT_QUIT:
            {
                loop.Stop();
                break;
            }
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                if (event.window.windowID == window->GetSDLWindowID()) loop.Stop();
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            {
                unsigned int w = window->GetWidth();
                unsigned int h = window->GetHeight();
                glad.Viewport(0,0,static_cast<int>(w),static_cast<int>(h));
                camera.SetAspect(static_cast<float>(w) / static_cast<float>(h));
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    window->SetMouseLockState(true);
                }
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        if (window->GetMouseLockState()) window->SetMouseLockState(false);
                        else loop.Stop();
                        
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
} // GoombaRender