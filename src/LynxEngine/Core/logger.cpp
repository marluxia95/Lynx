#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <stdarg.h>
#include <time.h>
#include <string>
#ifdef _WIN64
#include <stdbool.h>
#endif

#include "threadpool.h"
#include "application.h"
#include "logger.h"
#include "Utils/term.h"

namespace Lynx {

Logger logger;

void Logger::SetLevel(LogLevel level)
{
    m_level = level;
}

void Logger::Quiet(bool enable)
{
    quiet = enable;
}

void Logger::Log(LogLevel level, const std::string &text)
{
    char buf[16];
    time_t t = time(NULL);
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&t))] = '\0';

    std::unique_lock<std::mutex> lock(out_mutex);

    std::cout << "[" << std::string(buf) << getLevelColor(level) << " " << levelToString(level) << Utils::GetColorString(Utils::FG_GREEN) << getThreadName(std::this_thread::get_id()) << Utils::GetColorString(Utils::FG_WHITE) << "] " << text << std::endl;
}

void Logger::Log(LogLevel level, const char* fmt, ...)
{
    va_list va;
    char buf[512];
    va_start(va, fmt);

    vsnprintf(buf, 512, fmt, va);
    Log(level, std::string(buf));

    va_end(va);
}

void Logger::RegisterThread(std::thread::id t_id, const std::string &name)
{
    thread_name_map.insert({t_id, name});
}

std::string Logger::levelToString(LogLevel level)
{
    switch(level){
        case LOG_DEBUG:
            return "DEBUG   ";
        case LOG_INFO:
            return "INFO    ";
        case LOG_WARN:
            return "WARNING ";
        case LOG_ERROR:
            return "ERROR   ";
        case LOG_FATAL:
            return "FATAL   ";
    }
}

std::string Logger::getThreadName(std::thread::id t_id)
{
    if(thread_name_map.find(t_id) != thread_name_map.end())
        return thread_name_map[t_id];

    return "";
}

std::string Logger::getLevelColor(LogLevel level)
{
    Utils::ColorType color;
    switch(level){
        case LOG_DEBUG:
            color = Utils::FG_BLUE;
            break;
        case LOG_INFO:
            color = Utils::FG_WHITE;
            break;
        case LOG_WARN:
            color = Utils::FG_ORANGE;
            break;
        default:
            color = Utils::FG_RED;
    }
    return GetColorString(color);
}

}
