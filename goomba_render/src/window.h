#ifndef WINDOW_H
#define WINDOW_H

#include <utility>
#include <glfw/glfw3.h>

namespace GoombaRender
{
    class Window final
    {
    public:
        Window() = default;
        Window(int width, int height, const char* title, void(*func)());
        ~Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&& other) noexcept;
        Window& operator=(Window&& other) noexcept;

        inline GLFWwindow* GetHandle() const 
        {
            return m_Handle;
        }

        inline void MakeContextCurrent() const { glfwMakeContextCurrent(m_Handle); }
        inline void SwapBuffers() const { glfwSwapBuffers(m_Handle); }
        inline bool ShouldClose() const { return glfwWindowShouldClose(m_Handle); }
        inline std::pair<int, int> GetSize() const
        {
            int w,h;
            glfwGetFramebufferSize(m_Handle, &w, &h);
            return { w, h };
        }

    private:
        GLFWwindow* m_Handle = nullptr;
    };

    void CreateDefaultOpenGLContext();
}

#endif // WINDOW_H