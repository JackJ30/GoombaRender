#ifndef GLAD_CONTEXT_CREATOR_H
#define GLAD_CONTEXT_CREATOR_H

#include <glad/gl.h>
#include <glfw/glfw3.h>

namespace GoombaRender
{
    void ConfigureGLFWOpenGLContext(GLFWwindow* window);
    void CreateGLFWOpenGLContext(GLFWwindow* window);
}

#endif // GLAD_CONTEXT_CREATOR_H