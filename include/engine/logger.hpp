#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iomanip>


namespace Engine{
    class Logging {
        public:
            static void Init(const std::string &logFile="logs/log.log", const bool additional_log = true){
                auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);

                file_sink->set_level(spdlog::level::trace);

                std::vector<spdlog::sink_ptr> sinks{file_sink};

                if (additional_log){
                    auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    auto time = std::localtime(&current_time);
                    std::stringstream ss;
                    ss << std::put_time(time, "%Y_%m_%d_%H_%M_%S" );
                    auto add_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/"+ ss.str() +".log");
                    sinks.push_back(add_sink);
                }

                auto logger = std::make_shared<spdlog::logger>("multi_sink", sinks.begin(), sinks.end());
                spdlog::set_default_logger(logger);

                spdlog::set_pattern("[%^%l%$] [%Y-%m-%d %H:%M:%S.%e] [%@] %v");
                spdlog::set_level(spdlog::level::info);
                spdlog::flush_on(spdlog::level::warn);
            }
    };
} // namespace Engine
#endif // LOGGING_HPP
