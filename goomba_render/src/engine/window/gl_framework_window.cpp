#include "gl_framework_window.h"

#include "engine/events/input_event.h"
#include "engine/events/window_event.h"

namespace GoombaEngine
{
    static size_t s_Count = 0;

    GLFrameworkWindow::GLFrameworkWindow(const WindowProps& props, void(*configureGraphicsContextSettings)(GLFWwindow*), void(*createGraphicsContext)(GLFWwindow*))
    {
        GLogTrace("Creating GLFW window...");

        m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

        if (s_Count == 0)
        {
            glfwSetErrorCallback([](int error_code, const char* description)
            {
                GLogCritical("glfw error {:#x}: {}", error_code, description);
            });

            if (!glfwInit())
            {
                GLogCritical("failed to initialize glfw");
            }
        }
        else { GLogCritical("GoombaEngine does NOT currently support multiple windows."); } // TODO - Make it support multiple windows

        ++s_Count;

        if (configureGraphicsContextSettings) configureGraphicsContextSettings(m_Handle);

        // Create window
        m_Handle = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_Handle, &m_Data);
        GLogInfo("GLFWWindow '{}' created", m_Data.Title);

        // Create opengl context
        if (createGraphicsContext) createGraphicsContext(m_Handle);
        SetVSync(true);

        // Events
        BindEvents();
    }

    GLFrameworkWindow::~GLFrameworkWindow()
    {
        Shutdown();
    }

    GLFrameworkWindow::GLFrameworkWindow(GLFrameworkWindow &&other) noexcept
    {
        GLFrameworkWindow::operator=(std::move(other));
    }

    GLFrameworkWindow &GLFrameworkWindow::operator=(GLFrameworkWindow &&other) noexcept
    {
        GLFrameworkWindow::~GLFrameworkWindow();
        m_Handle = other.m_Handle;
        other.m_Handle = nullptr;

        m_Data = other.m_Data;

        return *this;
    }

    void GLFrameworkWindow::SetVSync(bool enabled)
    {
        // TODO - Support multiple windows

        glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
    }

    void GLFrameworkWindow::Shutdown()
    {
        if (m_Handle)
        {
            glfwDestroyWindow(m_Handle);
            m_Handle = nullptr;

            --s_Count;

            if (s_Count == 0)
            {
                glfwTerminate();
                glfwSetErrorCallback(nullptr);
            }

            GLogInfo("GLFWWindow terminated");
        }
    }

    void GLFrameworkWindow::BindEvents()
    {
        glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
        
        GLogInfo("GLFWWindow '{}' events bound", m_Data.Title);
    }

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
}