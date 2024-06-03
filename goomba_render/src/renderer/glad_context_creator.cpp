#include "glad_context_creator.h"

namespace GoombaRender
{
    // OpenGL + GLFW

    void ConfigureGLFWOpenGLContext(GLFWwindow* window)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        GLogInfo("Configured GLFW OpenGL Context");
    }

    void CreateGLFWOpenGLContext(GLFWwindow* window)
    {
        GLogTrace("Creating OpenGL GLFW Context...");

        glfwMakeContextCurrent(window);
        if (!gladLoadGL(glfwGetProcAddress))
        {
            GLogCritical("failed to load OpenGL");
            return;
        }

        GLogInfo("Created OpenGL GLFW Context");
    }

    // Supporting other rendering APIs is not something I am prioritizing at all, but it might be cool to have in the future so I'm (futilely) leaving the door open in some places.
}