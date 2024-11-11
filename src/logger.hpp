#pragma once

#include <chrono>
#include <fmt/core.h>
#include <fmt/chrono.h>

namespace debug_custom {

    /**
     * @enum LogLevel
     * @brief Defines the severity levels for logging messages
     * 
     * @var LogLevel::SUCCESS
     * @brief Indicates a successful operation (displayed as 'S')
     * 
     * @var LogLevel::INFO
     * @brief Indicates an informational message (displayed as 'I')
     * 
     * @var LogLevel::ERROR
     * @brief Indicates an error condition (displayed as 'E')
     */
    enum class LogLevel {
        SUCCESS,
        INFO,
        ERROR
    };

    /**
     * @brief Logs a formatted message with timestamp and severity level
     * 
     * @tparam Args Variadic template parameter for format arguments
     * @param level The severity level of the log message
     * @param format The format string using {fmt} library syntax
     * @param args The arguments to be formatted into the message
     * 
     * @details This function:
     *          - Adds a timestamp in YYYY-MM-DD HH:MM:SS format
     *          - Prepends a single-character severity indicator
     *          - Formats the message using the provided format string and arguments
     * 
     * Example usage:
     * @code{.cpp}
     * debug_custom::log(LogLevel::INFO, "Processing file: {}", filename);
     * debug_custom::log(LogLevel::ERROR, "Failed to open file: {}, error: {}", filename, err);
     * debug_custom::log(LogLevel::SUCCESS, "Processed {} records", count);
     * @endcode
     * 
     * @note This function uses the {fmt} library for string formatting and
     *       automatically flushes the output after each log message.
     */
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