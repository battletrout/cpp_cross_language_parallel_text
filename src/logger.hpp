#pragma once

#include <chrono>
#include <fmt/core.h>
#include <fmt/chrono.h>

namespace debug_custom {

    enum class LogLevel {
        SUCCESS,
        INFO,
        ERROR
    };

    template<typename... Args>
    void log(LogLevel level, const std::string_view format, Args&&... args) {
        auto now = std::chrono::system_clock::now();
        const char* level_str;
        
        switch(level) {
            case LogLevel::SUCCESS: level_str = "S"; break;
            case LogLevel::INFO:    level_str = "I"; break;
            case LogLevel::ERROR:   level_str = "E"; break;
        }

        fmt::print("{:%Y-%m-%d %H:%M:%S} [{}]: {}\n",
            now,
            level_str,
            fmt::format(format, std::forward<Args>(args)...));
    }




}