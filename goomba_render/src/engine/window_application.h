#ifndef WINDOW_APPLICATION_H
#define WINDOW_APPLICATION_H

#include "goombapch.h"

#include "engine/application.h"
#include "engine/window/window.h"

#include "engine/events/window_event.h"

namespace GoombaEngine
{
    class WindowApplication : public Application
    {
    public:
        void Run() override final;

    protected:
        std::unique_ptr<GoombaEngine::Window> m_Window;

    private:
        void OnEvent(Event& event);
        bool OnWindowClose(WindowCloseEvent& event);
    };
}

#endif // WINDOW_APPLICATION_H