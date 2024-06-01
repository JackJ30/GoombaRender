#ifndef LOG_H
#define LOG_H

#include <spdlog/spdlog.h>

namespace GoombaEngine
{
    std::shared_ptr<spdlog::logger>& GetLogger();

    void InitLogger();
}

#define GOOMBA_TRACE(args)  GoombaEngine::GetLogger()->trace(args)
#define GOOMBA_INFO(args)   GoombaEngine::GetLogger()->info(args)
#define GOOMBA_WARN(args)   GoombaEngine::GetLogger()->warn(args)
#define GOOMBA_ERROR(args)  GoombaEngine::GetLogger()->error(args)
#define GOOMBA_CRITICAL(args)  GoombaEngine::GetLogger()->critical(args)

#endif // LOG_H