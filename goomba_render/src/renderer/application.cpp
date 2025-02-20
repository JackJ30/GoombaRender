#include "application.h"

#include "graphics_context.h"
#include "engine/application_loop.h"
#include "engine/input.h"
#include "engine/imgui_layer.h"
#include "renderer/resource_manager.h"
#include "renderer/renderer.h"
#include "renderer/perspective_camera.h"

#include <glm/gtx/string_cast.hpp>

namespace GoombaRender
{
    std::unique_ptr<GoombaEngine::Window> window;
    GoombaEngine::ApplicationLoop loop;
    GoombaEngine::Input input;
    Renderer renderer;
    Scene testScene;
    
    PerspectiveCamera camera{{0.0, 0.0, 1.0}};
    
    void RunApplication()
    {
        GLogInfo("Application starting..");
        
        window = std::make_unique<GoombaEngine::Window>(GoombaEngine::WindowProperties{});
        window->RegisterEventCallback(&OnEvent);
        GoombaEngine::ImGUISetup(*window);
        
        // Load GLAD functions pointers for the context
        LoadGraphicsContext(window->GetProcAddress());
        
        // Setup Renderer
        renderer.Initialize();
        
        // Setup Loop
        loop.RegisterRenderCallback(&Render);
        loop.RegisterTickCallback(&Tick);
        
        camera.SetAspect(static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()));
        
        /*float vertices[4 * 5] = {
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
        
        std::shared_ptr<VertexArrayInfo> array = std::make_unique<VertexArrayInfo>(CreateVertexArray());
        array->BindBufferLayout(vbo, layout);
        IndicesSection indicesSection = {0, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT};
        array->SetIndexBuffer(ibo, {indicesSection});
        
        std::shared_ptr<Material> material = std::make_shared<Material>(LoadShader("resources/shaders/test.glsl"));
        material->AssignUniformTexture("u_Texture", LoadTexture2D("resources/images/goomba.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT));
        Mesh squareMesh(array, material);
        std::shared_ptr<Model> testModel = std::shared_ptr<Model>(new Model({squareMesh}, {vbo, ibo}));
        testScene.m_Objects.emplace_back(testModel, Transform());*/
        
        testScene.m_Objects.emplace_back(LoadModel("resources/models/AntiqueCamera.glb", LoadShader("resources/shaders/default.glsl")), Transform(glm::rotate(glm::mat4(1.0), glm::pi<float>(), {0.0, 0.0, 1.0})));
        testScene.m_Objects.emplace_back(LoadModel("resources/models/cubes.gltf", LoadShader("resources/shaders/default.glsl")), Transform());
        
        // LOOP
        loop.Run();
        // LOOP
        
        GLogInfo("Application shutting down...");
        GoombaEngine::ImGUIShutdown();
    }
    
    void Render(double delta, double interpolation)
    {
        window->PollEvents();
        GoombaEngine::ImGUIStartFrame();
        
        {
            renderer.AddScenePass(camera, testScene);
            renderer.Render();
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
    
    void Tick(double delta)
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