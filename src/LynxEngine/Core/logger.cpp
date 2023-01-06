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

namespace Lynx {

Console console;

void Console::Init()
{
     buffer.text = (char*)malloc(CONSOLE_BUFSIZE+1);
     buffer.lines = (con_lineinfo*)malloc(sizeof(con_lineinfo)*CONSOLE_MAXLINES+1);
     printf("console initialized at %x\n", buffer.text);
}

void Console::SetLevel(LogLevel level)
{
    m_level = level;
}

void Console::Quiet(bool enable)
{
    quiet = enable;
}

void Console::DeleteFirst()
{
    if(buffer.linecount==0) return;
    buffer.linecount--;
    buffer.firstline = (buffer.firstline + 1) % CONSOLE_MAXLINES;
}

void Console::DeleteLast()
{
    if(buffer.linecount==0) return;
    buffer.linecount--;
}

char* Console::getLevelStr(LogLevel level)
{
    switch(level) {
    case LOG_DEBUG: return "D";
    case LOG_WARN:  return "W";
    case LOG_ERROR: return "E";
    case LOG_FATAL: return "F";
    default: return "I";
    }
}

Console::con_lineinfo *Console::getLine(int n)
{
    return &buffer.lines[(buffer.firstline + n) % CONSOLE_MAXLINES];
}

Console::con_lineinfo *Console::getTail()
{
    return getLine(buffer.linecount-1);
}

// Checks if there is enough space for a line of len length
char* Console::BytesLeft(int len)
{
    if(len > CONSOLE_BUFSIZE)
	return NULL;
    
    if(buffer.linecount == 0)
	return buffer.text;

    char *first = getLine(buffer.firstline)->start;
    char *tail = getTail()->start + getTail()->len;
    if(first < tail) {
        // Place the string at the last line
        if(len <= buffer.text + CONSOLE_BUFSIZE - tail){
            return tail;
            // Place the string at the beggining of the buffer
        }else if(len <= tail - buffer.text){
            return buffer.text;
        }else{
            return NULL; // No space left
        }
    }else{ // The last line is behind the first line 
        if(len <= first - tail){ // There is enough space
            return tail;
        }else{
            return NULL;
        }
    }
}

void Console::PushLine(LogLevel level, const char* str)
{
    if(quiet) return;
    std::unique_lock<std::mutex> lock(out_mutex);
  
    char buf[CONSOLE_INPUTSIZE+1];
    char* putpos;
    int bufpos = 0;

    for(; *str; str++) {
        switch(*str) {
        case 0:
            break;
        case '\n':
            break;
        default:
            buf[bufpos++] = *str;
            if(bufpos >= CONSOLE_INPUTSIZE) {
                break;
            }
        }
    }
    putpos = BytesLeft(bufpos+1);
    if(!putpos || buffer.linecount >= CONSOLE_MAXLINES)
        DeleteFirst();

    memcpy(putpos, buf, bufpos);
    putpos[bufpos++] = 0;
    buffer.linecount++;
    con_lineinfo* l = getTail();
    l->level = level;
    l->start = putpos;
    l->len = bufpos;
}


void Console::Log(LogLevel level, const char* fmt, ...)
{
    if (level > m_level) return;

    va_list va;

    char buf[CONSOLE_INPUTSIZE];
    va_start(va, fmt);

    vsnprintf(buf, CONSOLE_INPUTSIZE, fmt, va);

    if(level > LOG_ERROR){
        fprintf(stdout, getLevelStr(level));
        fprintf(stdout, " "); // Shitty way
        fprintf(stdout, buf);
    }else{
        fprintf(stderr, getLevelStr(level));
        fprintf(stderr, " "); 
        fprintf(stderr, buf);
    }

    printf("\n");
    va_end(va);
}

int Console::GetLastLine()
{
	return buffer.linecount;
}

void Console::PrintBuffer()
{
    for(int l = 0; l < buffer.linecount; l++)
    {
        char buf[CONSOLE_INPUTSIZE];
        con_lineinfo *line = getLine(l);
        strncpy(buf, line->start, line->len);
        printf("(%d/%d)%s\n", l, CONSOLE_MAXLINES, buf);
    }
}

void Console::Render()
{
    float scale = 0.5f;
    float w = (float)Application::GetSingleton()->GetResolutionWidth();
    float h = (float)Application::GetSingleton()->GetResolutionHeight();
    auto font = Application::GetSingleton()->GetFontManager()->GetDefaultFont();
    Graphics::DrawFill(0, h-300, w, 300, glm::vec4(0, 0, 0, 0.5f));
    Graphics::DrawFill(0, h-316, w, 16, glm::vec4(0,0,0,0.7f));
    char ver[20];
    snprintf(ver, 20, "Lynx %d.%d.%d", LYNX_VERSION_MAJOR, LYNX_VERSION_MINOR, LYNX_VERSION_RELEASE);
    Graphics::DrawString(0, h-310, ver, font, scale, glm::vec3(1,0,0), 1);

    PrintBuffer();
    if(buffer.linecount==0)
        return;

    int maxlines = 300/16;
    for(int l = 0; l < maxlines; l++)
    {
        char buf[CONSOLE_INPUTSIZE];
        con_lineinfo *line = getLine(buffer.linecount-l);
        strncpy(buf, line->start, line->len);
        Graphics::DrawString(0, h-290+l*16, buf, font, scale, glm::vec3(1),1);
    }
}

void Console::Shutdown()
{
    free(buffer.text);
    free(buffer.lines);
}
}
