#ifndef ASSERT_H
#define ASSERT_H

#include "logger.h"

#ifdef __linux__
#define LYNX_ASSERT(condition, message) if(!(condition)) { log_fatal("Assertion %s failed in %s line %d : %s", #condition, __PRETTY_FUNCTION__, __LINE__, message); exit(-1); }
#elif _WIN32
#define LYNX_ASSERT(condition, message) if(!(condition)) { log_fatal("Assertion %s failed in %s line %d : %s", #condition, __FUNCDNAME__, __LINE__, message); exit(-1); }
#endif

#endif