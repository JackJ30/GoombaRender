#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "engine/engine.h"
#include "engine/window_application.h"
#include "renderer/glad_context_creator.h"

class Game : public GoombaEngine::Application { 
public:
    Game() = default;
    virtual ~Game() = default;

private:
    SDL_Window* window;
    SDL_Event event;

    virtual void OnInit() override
    {
        int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result < 0)
        {
            GLogCritical("SDL failed to initialize: {}", SDL_GetError());
        }

        GoombaRender::ConfigureSDLOpenGLContext();

        window = SDL_CreateWindow("Goomba Render", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (result < 0)
        {
            GLogCritical("SDL failed to create window: {}", SDL_GetError());
        }

        GoombaRender::CreateSDLOpenGLContext(window);
    }

    virtual void OnUpdate() override
    {
        while (SDL_PollEvent (&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_QUIT: 
                {
                    Stop();
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

        glClearColor(0.1f, 0.2f, 0.3f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
    }

    virtual void OnFinish() override
    {
        SDL_DestroyWindow(window);
    }
};

int main(int argc, char *argv[])
{
    Game game;
    GoombaEngine::RunApplication(game);

    return 0;
}

// TODO LIST
// ---------
// - [x] Logger
// - [x] Add event system
// - [x] Move opengl context stuff to a subclass of window? Or maybe it should be in a renderer class (update: I meod it to glad_context_creator and used another function pointer in glfwwindow)
// - [ ] Add layers
// - [ ] Escape to exit window
// - [ ] ImGUI
// - [ ] 3D Rendering Layer
// - [ ] Add support for resizing the window
// - [ ] Render loop and tick system
// - [ ] Basic OpenGL Abstractions
// - [ ] Render pipeline
// - [ ] Rewrite event dispatching, add event queue/bus
// - [ ] State system?
// - [ ] Scene system
// - [ ] Material system
// - [ ] Deferred rendering for specific materials
// - [ ] Move to sdl?
//
// Wishlist
// --------
// - [ ] Ray tracing
// - [ ] Offline rendering

// DEV LOG
// -------
// 6/1/2024 - I found a conflict between stb image and the fmt included by spdlog. This is the third times fmt has caused issues for me. I had to add a definition before including spdlog in the pch that I found in some github issue. Don't know what it does.
// 6/1/2024 - I'm thinking about what to do for events/layers/windows. TheCherno's system from the Hazel series seems a little bit overkill. I will probably add some of it it.