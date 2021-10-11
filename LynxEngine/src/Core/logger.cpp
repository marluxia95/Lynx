#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
#include <time.h>
#include "threadpool.h"
#include "application.h"
#include "logger.h"
#ifdef _WIN64
#include <stdbool.h>
#endif


static struct {
	LogLevel level;
	bool quiet;
	const char* outputFile;
	int errors, warnings;
} Logger;

static const char* levelStrings[] = {
	"FATAL  ", "ERROR  ", "WARNING", "INFO   ", "DEBUG  "
};

#ifdef __linux__
static const char* level_colours[] = {
  "\x1b[31m", "\x1b[31;1m", "\x1b[33;1m", "\x1b[37m", "\x1b[34;1m"
};

const char* colour_clear = "\x1b[0m";
#elif _WIN32
static const char* level_colours[] = {
  "[94m", "[36m", "[32m", "[33m", "[31m", "[35m"
};
const char* colour_clear = "[0m";
#endif

static void event_init(log_Event* ev) 
{
	time_t t = time(NULL);
	ev->time = localtime(&t);
}

void log_print(log_Event* ev) 
{
	if(ev == nullptr | ev->format == nullptr | ev->ap == NULL)
		return;
	char buf[16];
	buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';

	if(Lynx::Application::GetInstance() != nullptr) {
		if(Lynx::Application::GetInstance()->GetThread() == ev->th_id){
			printf("%s %s%s\x1b[0m \033[32mMAIN\x1b[0m  ",buf, level_colours[ev->level], levelStrings[ev->level] );
		}else{
			
			printf("%s %s%s\x1b[0m \033[33mWORKER%d\x1b[0m  ",
			buf, 
			level_colours[ev->level], 
			levelStrings[ev->level], 
			Lynx::GameApplication::GetGameInstance()->GetThreadPool()->GetWorkerID(ev->th_id));
		}
	}else{
		printf("%s %s%s\x1b[0m ",buf, level_colours[ev->level], levelStrings[ev->level] );
	}
	
	vprintf(ev->format, ev->ap);
	printf("%s\n", colour_clear);
}

const char* log_level_to_string(LogLevel level) 
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


