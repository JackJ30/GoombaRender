#include "engine.h"

namespace GoombaEngine
{
    void RunApplication(Application& application)
    {
        InitLogger();
        GOOMBA_INFO("Starting Application...");
        application.Start(); // TODO - The friend class thing isn't working so I can't put the start logic in this function. Do that ASAP
    }
}