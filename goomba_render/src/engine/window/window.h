#ifndef GOOMBARENDER_WINDOW_H
#define GOOMBARENDER_WINDOW_H

#include "engine/graphics_context.h"

#include <glad/gl.h>

namespace GoombaEngine
{
    struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		WindowProperties(const std::string& title = "GoombaRender",
			        unsigned int width = 1280,
			        unsigned int height = 720,
					bool vSync = true)
			: Title{title}, Width{width}, Height{height}, VSync{vSync}
		{
		}
	};

    class Window
    {
    public:
        virtual ~Window() {};

        virtual void PollEvents() = 0;
        virtual void SwapBuffers() = 0;

        virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual GraphicsContext & GetGraphicsContext() = 0;

		// Window attributes
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;
    };
}

#endif // WINDOW_H