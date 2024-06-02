#ifndef GLTF_WINDOW_H
#define GLTF_WINDOW_H

#include "window.h"

#include <glfw/glfw3.h>

namespace GoombaEngine
{
    class GLFrameworkWindow : public Window // Named it this because I couldn't name it "GLFWWindow" (I technically could have, but it would have been unclear)
    {
    public:
        GLFrameworkWindow(const WindowProps& props, void(*createGraphicsContext)());
        virtual ~GLFrameworkWindow();

        GLFrameworkWindow(const GLFrameworkWindow&) = delete;
        GLFrameworkWindow& operator=(const GLFrameworkWindow&) = delete;
        GLFrameworkWindow(GLFrameworkWindow&& other) noexcept;
        GLFrameworkWindow& operator=(GLFrameworkWindow&& other) noexcept;

        // Properties
        inline GLFWwindow* GetHandle() const { return m_Handle; }

        inline void Update() override { glfwPollEvents(); } // TODO - Support multiple windows
        inline void SwapBuffers() override { glfwSwapBuffers(m_Handle); }
        inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		void SetVSync(bool enabled) override;
		inline bool IsVSyncEnabled() const override { return m_Data.VSync; }
        
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        
        // GLFW Abstraction
        inline void MakeContextCurrent() { glfwMakeContextCurrent(m_Handle); }
        inline static GLFWglproc GetProcAddress(const char *procName) { return glfwGetProcAddress(procName); }
    
    private:
		virtual void Shutdown();
        
        void BindEvents();
    
    private:
        GLFWwindow* m_Handle = nullptr;

        struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
    };

    void CreateDefaultOpenGLContext();
}

#endif // GLTF_WINDOW_H