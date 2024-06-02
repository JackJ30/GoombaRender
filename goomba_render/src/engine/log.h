#ifndef LOG_H
#define LOG_H

#include "goombapch.h"

#include <spdlog/spdlog.h>

namespace GoombaEngine
{
    std::shared_ptr<spdlog::logger>& GetLogger();

    void InitLogger();
}

#define GLogTrace(args...)     GoombaEngine::GetLogger()->trace(args)
#define GLogInfo(args...)      GoombaEngine::GetLogger()->info(args)
#define GLogWarn(args...)      GoombaEngine::GetLogger()->warn(args)
#define GLogError(args...)     GoombaEngine::GetLogger()->error(args)
#define GLogCritical(args...)  GoombaEngine::GetLogger()->critical(args)

#endif // LOG_H