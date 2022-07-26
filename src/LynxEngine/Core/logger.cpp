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

static std::mutex out_mutex;

static struct {
	LogLevel level = LOG_INFO;
	bool quiet;
	std::string outputFile;
	int errors, warnings;
} Logger;

static std::string levelStrings[] = {
	"FATAL  ", "ERROR  ", "WARNING", "INFO   ", "DEBUG  "
};

static std::map<LogLevel, Utils::ColourType> log_colours = {
	{LOG_DEBUG,   Utils::FG_BLUE},
	{LOG_INFO,    Utils::FG_WHITE},
	{LOG_WARN,    Utils::FG_ORANGE},
	{LOG_ERROR,   Utils::FG_RED},
	{LOG_FATAL,   Utils::FG_RED}
};

static void event_init(log_Event* ev) 
{
	time_t t = time(NULL);
	ev->time = localtime(&t);
}

std::string getLevelColour(LogLevel lvl)
{
	return Utils::GetColourString(log_colours[lvl]);
}

void log_print(log_Event* ev) 
{
	if(ev == nullptr | ev->format == nullptr | ev->ap == NULL)
		return;
	char buf[16];
	buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';

	std::unique_lock<std::mutex> lock(out_mutex);

	std::cout << std::string(buf) << getLevelColour(ev->level) << " " << levelStrings[ev->level] << Utils::GetColourString(Utils::FG_WHITE) <<
					" ";
	
	vprintf(ev->format, ev->ap);
	std::cout << Utils::GetColourString(Utils::FG_WHITE) << std::endl;
}

std::string log_level_to_string(LogLevel level) 
{
	return levelStrings[level];
}

void log_set_level(LogLevel level) 
{
	Logger.level = level;
}

void log_quiet(bool enable) 
{
	Logger.quiet = enable;
}

int log_geterrorcount()
{
	return Logger.errors;
}

int log_getwarningcount()
{
	return Logger.warnings;
}


void log_log(std::thread::id th_id, LogLevel level, const char* format, ...) 
{
	log_Event ev;
	ev.format = format;
	ev.level = level;
	ev.th_id = th_id;

	if (!Logger.quiet && level <= Logger.level | level == LOG_FATAL) {
		if(level == LOG_ERROR | level == LOG_FATAL)
			Logger.errors++;

		if(level == LOG_WARN)
			Logger.warnings++;

		event_init(&ev);
		va_start(ev.ap, format);
		log_print(&ev);
		va_end(ev.ap);
	}
}


