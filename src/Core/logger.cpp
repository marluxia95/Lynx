#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
#include <time.h>
#include <cstring>
#ifdef _WIN64
#include <stdbool.h>
#endif

#include "threadpool.h"
#include "application.h"
#include "logger.h"
#include "Graphics/draw.h"
#include "Utils/term.h"

Logger logger;
StreamSink log_stdout(LOG_INFO,	 std::cout);
StreamSink log_stderr(LOG_ERROR, std::cerr);
BufferSink log_conbuf(LOG_INFO);

static const char* levelStrings[] = {
	"FATAL", "ERROR", "WARNING", "INFO", "DEBUG"
};

void Logger::RegisterSink(LogSink *out)
{
	if(m_sinkCount>=LOG_MAXSINKS) { return; }
	m_sinks[m_sinkCount++] = out;
}	

void Logger::Log(LogLevel level, const char *fmt, ...)
{
	va_list va;

	char fbuf[LOG_LINESIZE];
	char logbuf[LOG_LINESIZE];
	
	va_start(va, fmt);
	vsnprintf(fbuf, LOG_LINESIZE, fmt, va);
	snprintf(logbuf, LOG_LINESIZE, " %s: %s", levelStrings[level], fbuf);

	for(int i = 0; i < m_sinkCount; i++) {
//		if(!m_sinkCount < i) continue;
		if(m_sinks[i]->GetLevel() < level) continue;
		m_sinks[i]->Log(logbuf);
	}
	
	va_end(va);
}

void StreamSink::Log(const char *str)
{

	m_stream << std::string(str) << std::endl;
}

void BufferSink::Log(const char *str)
{
	buf[index] = std::string(str);

	if(index++ == BUF_MAXLINES) {
	index = 0;
	}
}

std::string BufferSink::GetLine(size_t index)
{
	return buf[index];
}
