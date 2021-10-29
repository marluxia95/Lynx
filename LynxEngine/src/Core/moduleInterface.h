#ifndef LYNX_MODULE_INTERFACE_H
#define LYNX_MODULE_INTERFACE_H

#ifdef __linux__

#define ENGINE_MODULE_INIT int module_init()

#define ENGINE_MODULE_CLOSE int module_close()

#endif

#endif // LYNX_MODULE_INTERFACE_H