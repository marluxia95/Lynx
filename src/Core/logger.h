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
#include <fstream>
#include <stdbool.h>
#include <string>
#include <array>
#include <thread>
#include <queue>
#include <mutex>
#include <map>
#include "lynx_common.h"

#define LOG_MAXSINKS 16
#define LOG_LINESIZE 4096
#define BUF_MAXLINES 32768

typedef enum {
	LOG_FATAL,
	LOG_ERROR,
	LOG_WARN,
	LOG_INFO,
	LOG_DEBUG,
	LOG_ALL
} LogLevel;

class LogSink;
	
class LYNXENGINE_API Logger {
public:
	void Log(LogLevel level, const char* fmt, ...);
	void Log(LogLevel level, std::string str) {Log(level, str.c_str());}

	char* getLevelStr(LogLevel level);

	void RegisterSink(LogSink *out);
private:
	LogSink* m_sinks[LOG_MAXSINKS];
	short int  m_sinkCount = 0;
};

class LogSink {
public:
	LogSink(LogLevel level) : m_level(level) {}
	virtual void Log(const char* msg) = 0;
	LogLevel GetLevel() { return m_level; }
	void SetLevel(LogLevel level) { m_level = level; }
private:
	LogLevel m_level;
};

class StreamSink : public LogSink {
public:
	StreamSink(LogLevel level, std::ostream &stream) : LogSink(level), m_stream(stream) {}

	void Log(const char* msg) override;
private:
	std::ostream &m_stream;
};

class BufferSink : public LogSink {
public:
	BufferSink(LogLevel level) : LogSink(level) {}

	void Log(const char* msg) override;

	std::string GetLine(size_t index);
private:
	std::array<std::string, BUF_MAXLINES> buf;
	size_t index;
};

extern Logger logger;
extern StreamSink log_stdout;
extern StreamSink log_stderr;
extern BufferSink log_conbuf;

// TODO : Add log streams

#define log_debug(...) logger.Log(LOG_DEBUG, __VA_ARGS__)
#define log_info(...)  logger.Log(LOG_INFO,	 __VA_ARGS__)
#define log_warn(...)  logger.Log(LOG_WARN,	 __VA_ARGS__)
#define log_error(...) logger.Log(LOG_ERROR, __VA_ARGS__)
#define log_fatal(...) logger.Log(LOG_FATAL, __VA_ARGS__)

#endif
