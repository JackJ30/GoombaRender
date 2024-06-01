#include <iostream>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "window.h"
#include "application.h"

class Game : public GoombaRender::Application
{
public:
    Game() = default;
    virtual ~Game() = default;

    virtual void Init() override
    {
        m_Window = GoombaRender::Window(1280, 720, "Goomba Render", GoombaRender::CreateDefaultOpenGLContext);

        if (!m_Window.GetHandle())
        {
            spdlog::critical("failed to create window");
            Stop();
            return;
        }

        m_Window.MakeContextCurrent();

        if (!gladLoadGL(glfwGetProcAddress))
        {
            spdlog::critical("failed to load OpenGL");
            Stop();
            return;
        }
    }

    virtual void Update() override
    {
        if (m_Window.ShouldClose()) Stop();

        glfwPollEvents();
            
        glClearColor(.3, 1, .3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Window.SwapBuffers();
    }

    virtual void Destroy() override
    {

    }

private:
    GoombaRender::Window m_Window;
};

int main(int argc, char *argv[])
{
    Game game;
    game.Start();
}