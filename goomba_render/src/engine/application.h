#ifndef APPLICATION_H
#define APPLICATION_H

#include "goombapch.h"

namespace GoombaEngine
{
    class Application
    {
    public:
        Application() = default;
        virtual ~Application() = default;

        virtual void Run();

        inline void Stop()
        {
            m_Running = false;
        }
    
    protected:
        bool m_Running = false;

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Finish() = 0;
    };
}

#endif // APPLICATION_H