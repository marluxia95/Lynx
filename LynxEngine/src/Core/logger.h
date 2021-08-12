#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>


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
	LogLevel level;
} log_Event;

const char* log_level_to_string(LogLevel level);
void log_set_level(LogLevel level);
void log_quiet(bool enable);
void log_log(LogLevel level, const char* format, ...);
int log_geterrorcount();
int log_getwarningcount();

#define log_debug(...) log_log(LOG_DEBUG, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __VA_ARGS__)


#endif