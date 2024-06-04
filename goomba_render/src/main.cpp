#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "engine/engine.h"
#include "engine/window_application.h"
#include "engine/window/sdl_window.h"

class Game : public GoombaEngine::WindowApplication { 
public:
    Game() = default;
    virtual ~Game() = default;

private:
    virtual void OnInit() override
    {

    }

    virtual void OnUpdate() override
    {
        m_Window->GetGladContext().ClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        m_Window->GetGladContext().Clear(GL_COLOR_BUFFER_BIT);
    }

    virtual void OnFinish() override
    {
        
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