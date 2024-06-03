#ifndef WINDOW_H
#define WINDOW_H

#include "goombapch.h"
#include "engine/events/event.h"

namespace GoombaEngine
{
    struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "GoombaRender",
			        unsigned int width = 1280,
			        unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

    class Window
    {
    public:
        virtual ~Window() {};

        virtual void Update() = 0;
        virtual void SwapBuffers() = 0;

        virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;
    };
}

#endif // WINDOW_H