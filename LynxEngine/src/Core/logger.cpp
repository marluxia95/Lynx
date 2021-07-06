#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
#include <time.h>
#include "logger.h"
#include "imgui.h"
#ifdef _WIN64
#include <stdbool.h>
#endif


static struct {
	LogLevel level;
	bool quiet;
	const char* outputFile;
	ImGuiTextBuffer buffer;
} Logger;

static const char* levelStrings[] = {
	"FATAL", "ERROR", "WARNING", "INFO", "DEBUG"
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
	if (!ev->time) {
		time_t t = time(NULL);
		ev->time = localtime(&t);
	}
}

void log_print(log_Event* ev) 
{
	char buf[16];
	buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
	printf("%s %s%s\x1b[0m \x1b[90m", buf, level_colours[ev->level], levelStrings[ev->level]);
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


static void console_log(log_Event* ev)
{
	char buf[16];
	buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
	char buffer[sizeof(ev->format)+256];
	sprintf(buffer, "%s %s ",  buf, levelStrings[ev->level]);
	
	Logger.buffer.append(buffer);
	memset(buffer, 0, sizeof(buffer)); // Clear buffer
	sprintf(buffer, ev->format, ev->ap);
	Logger.buffer.append(buffer);
	Logger.buffer.append("\n");
}

void log_log(LogLevel level, const char* format, ...) 
{
	log_Event ev;
	ev.format = format;
	ev.level = level;

	if (!Logger.quiet && level <= Logger.level) {
		event_init(&ev);
		va_start(ev.ap, format);
		log_print(&ev);
		console_log(&ev);
		va_end(ev.ap);
	}
}


void console_draw()
{
	ImGui::Begin("Console");

	ImGui::TextUnformatted(Logger.buffer.begin());

	ImGui::End();
}

