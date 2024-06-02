#ifndef APPLICATION_H
#define APPLICATION_H

#include "goombapch.h"

namespace GoombaEngine
{
    class Application
    {
        friend class Engine;

    public:
        Application() = default;
        virtual ~Application() = default;

        void Start();

        inline void Stop()
        {
            m_Running = false;
        }
    
    private:
        bool m_Running = false;

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Finish() = 0;
    };
}

#endif // APPLICATION_H