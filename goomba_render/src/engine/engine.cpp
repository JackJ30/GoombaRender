#include "engine.h"

namespace GoombaEngine
{
    void RunApplication(Application& application)
    {
        InitLogger();
        GLogInfo("Starting Application...");
        application.Run();
        GLogInfo("Shutting Down...");
    }
}