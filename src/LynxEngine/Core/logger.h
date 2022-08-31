/**
 * @file logger.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief This includes all the log handling 
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <string>
#include <thread>
#include <mutex>
#include <map>
#include "lynx_common.h"

namespace Lynx {

typedef enum {
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG
} LogLevel;

// TODO : Add log to file feature
class LYNXENGINE_API Logger {
public:
    void SetLevel(LogLevel level);
    void Quiet(bool enable);
    void Log(LogLevel level, const char* fmt, ...);
    void Log(LogLevel level, const std::string &text);
    void RegisterThread(std::thread::id t_id, const std::string &name);

private:
    std::string levelToString(LogLevel level);
    std::string getThreadName(std::thread::id t_id);
    std::string getLevelColor(LogLevel level);

    std::mutex out_mutex;

    std::map<std::thread::id, std::string> thread_name_map;
    LogLevel m_level = LOG_INFO;
    bool quiet = false;
};

extern Logger logger;

// TODO : Add log streams

}

#define log_debug(...) Lynx::logger.Log(Lynx::LOG_DEBUG, __VA_ARGS__)
#define log_info(...)  Lynx::logger.Log(Lynx::LOG_INFO,  __VA_ARGS__)
#define log_warn(...)  Lynx::logger.Log(Lynx::LOG_WARN,  __VA_ARGS__)
#define log_error(...) Lynx::logger.Log(Lynx::LOG_ERROR, __VA_ARGS__)
#define log_fatal(...) Lynx::logger.Log(Lynx::LOG_FATAL, __VA_ARGS__)

#endif
