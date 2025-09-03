#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>
#include <utility>


#include "common/logger.h"

/**
 * @brief 执行时间计算日记，作用域内有效
 */
class ExecuteTimer {
public:
    explicit ExecuteTimer(std::string  name = "")
        : name_(std::move(name)), start_(Clock::now()) {}

    ~ExecuteTimer() {
        const auto end = Clock::now();
        const std::chrono::duration<double> elapsed = end - start_;


        LOG_INFO("{} 耗时: {:.6f} 秒", 
                 (name_.empty() ? "代码块" : name_), elapsed.count());
    }

private:
    using Clock = std::chrono::steady_clock;
    std::string name_;
    std::chrono::time_point<Clock> start_;
};
