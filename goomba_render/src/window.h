#ifndef WINDOW_H
#define WINDOW_H

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
        inline bool ShouldClose() const { return glfwWindowShouldClose(m_Handle); }
        inline void MakeContextCurrent() { glfwMakeContextCurrent(m_Handle); }
        inline void SwapBuffers() { glfwSwapBuffers(m_Handle); }
        inline std::pair<int, int> GetSize() const
        {
            int w,h;
            glfwGetFramebufferSize(m_Handle, &w, &h);
            return { w, h };
        }

        inline static void PollEvents()
        {
            glfwPollEvents();
        }
        inline static GLFWglproc GetProcAddress(const char *procName) { return glfwGetProcAddress(procName); }

    private:
        GLFWwindow* m_Handle = nullptr;
    };

    void CreateDefaultOpenGLContext();
}

#endif // WINDOW_H