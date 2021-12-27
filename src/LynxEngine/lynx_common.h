#pragma once

#if _WIN32
#ifdef LynxEngine_EXPORTS
#define LYNXENGINE_API __declspec(dllexport) 
#else
#define LYNXENGINE_API __declspec(dllimport)
#endif
#else
#define LYNXENGINE_API 
#endif

#define LYNX_VERSION_MAJOR   0
#define LYNX_VERSION_MINOR   0
#define LYNX_VERSION_RELEASE 3
#define LYNX_VERSION LYNX_VERSION_MAJOR.LYNX_VERSION_MINOR.LYNX_VERSION_RELEASE

