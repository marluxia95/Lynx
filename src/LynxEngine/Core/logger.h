#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>
#include <string>
#include <thread>
#include "lynx_common.h"

typedef enum {
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG
} LogLevel;

typedef struct {
	va_list ap;
	const char* format;
	struct tm* time;
	std::thread::id th_id;
	LogLevel level;
} log_Event;

LYNXENGINE_API std::string log_level_to_string(LogLevel level);
LYNXENGINE_API void log_set_level(LogLevel level);
LYNXENGINE_API void log_quiet(bool enable);
LYNXENGINE_API void log_log(std::thread::id th_id, LogLevel level, const char* format, ...);
LYNXENGINE_API int log_geterrorcount();
LYNXENGINE_API int log_getwarningcount();

#define log_debug(...) log_log(std::this_thread::get_id(), LOG_DEBUG, __VA_ARGS__)
#define log_info(...)  log_log(std::this_thread::get_id(), LOG_INFO, __VA_ARGS__)
#define log_warn(...)  log_log(std::this_thread::get_id(), LOG_WARN, __VA_ARGS__)
#define log_error(...) log_log(std::this_thread::get_id(), LOG_ERROR, __VA_ARGS__)
#define log_fatal(...) log_log(std::this_thread::get_id(), LOG_FATAL, __VA_ARGS__)
#define log_pref(level, preffix, ...) log_log(std::this_thread::get_id(), level, __VA_ARGS__, preffix)
#endif