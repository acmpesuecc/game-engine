#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iomanip>
#include <chrono>
#include <iostream>

namespace Engine{
    class Logging {
        public:
            static void Init(const std::string &logFile="logs/log.log", const bool additional_log = true){
                std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink;
                try {
                    file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);
                    file_sink->set_level(spdlog::level::trace);
                }
                catch (const spdlog::spdlog_ex &ex) {
                    std::cerr << "Failed to create primary log file (file may be locked or inaccessible): " << ex.what() << std::endl;

                    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                    auto logger = std::make_shared<spdlog::logger>("console_logger", console_sink);

                    spdlog::set_default_logger(logger);
                    spdlog::warn("Logging to console instead of file due to file lock or access issues.");
                    return;
                }
                std::vector<spdlog::sink_ptr> sinks{file_sink};

                if (additional_log){
                    try {
                        auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                        auto time = std::localtime(&current_time);
                        std::stringstream ss;
                        ss << std::put_time(time, "%Y_%m_%d_%H_%M_%S");
                        auto add_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + ss.str() + ".log");
                        sinks.push_back(add_sink);
                    }
                    catch (const spdlog::spdlog_ex &ex) {
                        std::cerr << "Failed to create additional log file: " << ex.what() << std::endl; //file perms issue
                    }
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
