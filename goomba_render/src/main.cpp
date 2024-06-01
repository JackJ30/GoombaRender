#include <iostream>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include "window.h"


int main(int argc, char *argv[])
{
    spdlog::info("Hello World!");

    GoombaRender::Window window = GoombaRender::Window(1280, 720, "Goomba Render");
    window.MakeContextCurrent();

    gladLoadGL(glfwGetProcAddress);

    if (!window.ShouldClose())
    {
        glfwPollEvents();
        
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        window.SwapBuffers();
    }
}