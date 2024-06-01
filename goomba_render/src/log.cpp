#include "log.h"

#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace GoombaEngine
{
    static std::shared_ptr<spdlog::logger> renderLogger;

    std::shared_ptr<spdlog::logger>& GetLogger()
    {
        return renderLogger;
    }
    
    void InitLogger()
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/render.txt", true);
        file_sink->set_level(spdlog::level::trace);

        spdlog::sinks_init_list sink_list = { file_sink, console_sink };

        renderLogger = std::make_shared<spdlog::logger>("renderLogger", sink_list.begin(), sink_list.end());
    }
}