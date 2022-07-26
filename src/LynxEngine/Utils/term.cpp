#include "term.h"

namespace Utils {

#ifndef _WIN32 
const std::map<ColourType, std::string> colour_strings = {
                {FG_BLACK,      "\033[30m"},
                {FG_RED,        "\033[31m"},
                {FG_GREEN,      "\033[32m"},
                {FG_ORANGE,     "\033[33m"},
                {FG_BLUE,       "\033[34m"},
                {FG_MAGENTA,    "\033[35m"},
                {FG_CYAN,       "\033[36m"},
                {FG_LIGHT_GRAY, "\033[37m"},
                {FG_WHITE,      "\033[39m"},
                
                {BG_BLACK,      "\033[40m"},
                {BG_RED,        "\033[41m"},
                {BG_GREEN,      "\033[42m"},
                {BG_ORANGE,     "\033[43m"},
                {BG_BLUE,       "\033[44m"},
                {BG_MAGENTA,    "\033[45m"},
                {BG_CYAN,       "\033[436m"},
                {BG_LIGHT_GRAY, "\033[47m"},
                {BG_WHITE,      "\033[48m"},
                
                {FG_LIGHT_RED,  "\033[1;31"}
};

std::string GetColourString(ColourType colour) 
{
	auto p = colour_strings.find(colour);
	if(p == colour_strings.end())
		return "";
	else
		return p->second;
} 
#else

std::string GetColourString(ColourType colour) 
{
    return "";
} 

#endif



}
