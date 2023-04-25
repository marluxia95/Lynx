/**
 * @file logger.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief This includes all the log handling 
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
#include <queue>
#include <mutex>
#include <map>
#include "lynx_common.h"

#define CONSOLE_BUFSIZE 1048576
#define CONSOLE_MAXLINES 16384
#define CONSOLE_INPUTSIZE 16384

namespace Lynx {

typedef enum {
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
	LOG_ALL
} LogLevel;

/*
 * Quake-based console with a circular buffer
 *
 */
class LYNXENGINE_API Console {
public:
    struct con_lineinfo {
	LogLevel level;
	char* start;
	int len;
    };

    struct con_buffer {
	con_lineinfo* lines;
	char* text;
	int firstline = 0;
	int linecount = 0;
    };

    con_lineinfo *getLine(int n);
    con_lineinfo *getTail();

    char* getLevelStr(LogLevel level);
public:
    void Init();
    void SetLevel(LogLevel level);
    void Quiet(bool enable);
    void PushLine(LogLevel level, const char* str);
    void Log(LogLevel level, const char* fmt, ...);
    void Log(LogLevel level, std::string str) {Log(level, str.c_str());}
    void DeleteFirst();
    void DeleteLast();
    char* BytesLeft(int len);
    void PrintBuffer();
    
    void GetLine(int l);
    int GetLastLine();
    
    void Render();

    void Shutdown();
private:
    con_buffer buffer;
    std::mutex out_mutex;
    LogLevel m_level;
    bool quiet = false;
};

extern Console console;
// TODO : Add log streams

}

#define log_debug(...) Lynx::console.Log(Lynx::LOG_DEBUG, __VA_ARGS__)
#define log_info(...)  Lynx::console.Log(Lynx::LOG_INFO,  __VA_ARGS__)
#define log_warn(...)  Lynx::console.Log(Lynx::LOG_WARN,  __VA_ARGS__)
#define log_error(...) Lynx::console.Log(Lynx::LOG_ERROR, __VA_ARGS__)
#define log_fatal(...) Lynx::console.Log(Lynx::LOG_FATAL, __VA_ARGS__)

#endif
