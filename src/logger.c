#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"
#ifdef _WIN64
#include <stdbool.h>
#endif

static struct {
	LogLevel level;
	bool quiet;
	const char* outputFile;
} Logger;

static const char* levelStrings[] = {
	"FATAL", "ERROR", "WARNING", "INFO", "DEBUG"
};

#ifdef __linux__
static const char* level_colours[] = {
  "\x1b[31m", "\x1b[31;1m", "\x1b[33;1m", "\x1b[37m", "\x1b[34;1m"
};

const char* colour_clear = "\x1b[0m";
#elif _WIN64
static const char* level_colours[] = {
  "[94m", "[36m", "[32m", "[33m", "[31m", "[35m"
};
const char* colour_clear = "[0m";
#endif

static void event_init(log_Event* ev) {
	if (!ev->time) {
		time_t t = time(NULL);
		ev->time = localtime(&t);
	}
}

void log_print(log_Event* ev) {
	char buf[16];
	buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
	printf("%s %s%s\x1b[0m \x1b[90m", buf, level_colours[ev->level], levelStrings[ev->level]);
	vprintf(ev->format, ev->ap);
	printf("%s\n", colour_clear);
}

const char* log_level_to_string(int level) {
	return levelStrings[level];
}

void log_set_level(int level) {
	Logger.level = level;
}

void log_quiet(bool enable) {
	Logger.quiet = enable;
}

void log_log(int level, const char* format, ...) {
	log_Event ev = {
		.format = format,
		.level = level
	};

	if (!Logger.quiet && level >= Logger.level) {
		event_init(&ev);
		va_start(ev.ap, format);
		log_print(&ev);
		va_end(ev.ap);
	}
}

