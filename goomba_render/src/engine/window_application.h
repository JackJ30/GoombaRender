#ifndef WINDOW_APPLICATION_H
#define WINDOW_APPLICATION_H

#include "goombapch.h"

#include "engine/application.h"
#include "engine/window/window.h"

namespace GoombaEngine
{
    class WindowApplication : public Application
    {
    public:
        void Run() override;

    private:
        std::unique_ptr<GoombaEngine::Window> m_Window;
    };
}

#endif // WINDOW_APPLICATION_H