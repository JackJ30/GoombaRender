#include <iostream>
#include <glad/gl.h>

#include "window.h"
#include "engine.h"
#include "application.h"
#include "log.h"

class Game : public GoombaEngine::Application
{
public:
    Game() = default;
    virtual ~Game() = default;

private:
    virtual void Init() override
    {
        m_Window = GoombaRender::Window(1280, 720, "Goomba Render", GoombaRender::CreateDefaultOpenGLContext);

        if (!m_Window.GetHandle())
        {
            GOOMBA_CRITICAL("failed to create window");
            Stop();
            return;
        }

        m_Window.MakeContextCurrent();

        if (!gladLoadGL(GoombaRender::Window::GetProcAddress))
        {
            GOOMBA_CRITICAL("failed to load OpenGL");
            Stop();
            return;
        }
    }

    virtual void Update() override
    {
        if (m_Window.ShouldClose()) Stop();

        GoombaRender::Window::PollEvents();
            
        glClearColor(.3, 1, .3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Window.SwapBuffers();
    } 

    virtual void Finish() override
    {

    }

private:
    GoombaRender::Window m_Window;
};

int main(int argc, char *argv[])
{
    Game game;

    GoombaEngine::RunApplication(game);
}

// TODO LIST
// ---------
// - [x] Logger
// - [ ] Add event system
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