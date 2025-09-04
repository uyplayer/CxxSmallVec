#pragma once


#include <atomic>
#include <iostream>
#include <memory>



#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/**
 * @brief spdlog 日志初始化
 * 
 */
inline void logger_init() {
    try {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        auto logger = std::make_shared<spdlog::logger>("console", console_sink);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
        spdlog::flush_on(spdlog::level::info);
        spdlog::info("Logger 初始化完成 (console only)");
    }
    catch (const spdlog::spdlog_ex &ex) {
        std::cerr << "Logger 初始化失败: " << ex.what() << std::endl;
    }
}

#define LOG_TRACE(...)  SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...)  SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...)   SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)   SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)  SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)