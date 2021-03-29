#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum {
	LOG_FATAL = -2,
	LOG_ERROR = -1,
	LOG_INFO  = 0,
	LOG_WARN  = 1,
	LOG_DEBUG = 2
} LogLevel;

class Logger{
	public:
		Logger(const char* logFile, LogLevel level, bool isSilent);
		void log(LogLevel loglvl, const char* str);
	private:
		char* getDateString();
		const char* filePath;
		int Level;
		bool silent;

};

#endif LOGGER_H