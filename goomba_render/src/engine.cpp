#include "engine.h"
#include "log.h"

namespace GoombaEngine
{
    void RunApplication(Application& application)
    {
        InitLogger();
        application.Start(); // TODO - The friend class thing isn't working so I can't put the start logic in this function. Do that ASAP
    }
}