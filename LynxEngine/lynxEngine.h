#pragma once

#ifndef _WIN
#ifdef LYNXENGINE_EXPORTS
#define LYNXENGINE_API __declspec(dllexport)
#else
#define LYNXENGINE_API __declspec(dllimport)
#endif
#endif

#include "Core/simpleGameEngine.h"