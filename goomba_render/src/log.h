#ifndef LOG_H
#define LOG_H

#include <spdlog/spdlog.h>

namespace GoombaEngine
{
    std::shared_ptr<spdlog::logger>& GetLogger();

    void InitLogger();
}

#endif // LOG_H