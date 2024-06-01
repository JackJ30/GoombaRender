#include "window.h"

#include <spdlog/spdlog.h>

namespace GoombaRender
{
    static size_t s_Count = 0;

    Window::Window(int width, int height, const char *title, void(*createContext)() = CreateDefaultOpenGLContext)
    {
        if (s_Count == 0)
        {
            glfwSetErrorCallback([](int error_code, const char* description)
            {
                spdlog::error("glfw error {:#x}: {}", error_code, description);
            });

            if (!glfwInit())
            {
                spdlog::error("failed to initialize glfw");
            }
        }

        ++s_Count;

        if (createContext) createContext();

        m_Handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    Window::~Window()
    {
        if (m_Handle)
        {
            if (m_Handle) glfwDestroyWindow(m_Handle);
            m_Handle = nullptr;

            --s_Count;

            if (s_Count == 0)
            {
                glfwTerminate();
                glfwSetErrorCallback(nullptr);
            }
        }
    }

    Window::Window(Window &&other) noexcept
    {
        Window::operator=(std::move(other));
    }

    Window &Window::operator=(Window &&other) noexcept
    {
        Window::~Window();
        m_Handle = other.m_Handle;
        other.m_Handle = nullptr;

        return *this;
    }
    
    void CreateDefaultOpenGLContext()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    }
}