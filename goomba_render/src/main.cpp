#include <glad/gl.h>

#include "engine/window.h"
#include "engine/engine.h"
#include "engine/application.h"

class Game : public GoombaEngine::Application
{
public:
    Game() = default;
    virtual ~Game() = default;

private:
    virtual void Init() override
    {
        m_Window = GoombaEngine::Window(1280, 720, "Goomba Render", GoombaEngine::CreateDefaultOpenGLContext);

        if (!m_Window.GetHandle())
        {
            GLogCritical("failed to create window");
            Stop();
            return;
        }

        m_Window.MakeContextCurrent();

        if (!gladLoadGL(GoombaEngine::Window::GetProcAddress))
        {
            GLogCritical("failed to load OpenGL");
            Stop();
            return;
        }
    }

    virtual void Update() override
    {
        if (m_Window.ShouldClose()) Stop();

        GoombaEngine::Window::PollEvents();
            
        glClearColor(.3, 1, .3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Window.SwapBuffers();
    } 

    virtual void Finish() override
    {

    }

private:
    GoombaEngine::Window m_Window;
};

int main(int argc, char *argv[])
{
    Game game;

    GoombaEngine::RunApplication(game);
}

// TODO LIST
// ---------
// - [x] Logger
// - [x] Add event system
// - [ ] Move opengl context stuff to a subclass of window? Or maybe it should be in a renderer class
// - [ ] Add support for resizing the window
// - [ ] ImGUI
// - [ ] Escape to exit window
// - [ ] Render loop and tick system
// - [ ] Basic OpenGL Abstractions
// - [ ] Render pipeline
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