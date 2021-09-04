#ifndef LYNX_H
#define LYNX_H

#ifndef _WIN
#ifdef LYNXENGINE_EXPORTS
#define LYNXENGINE_API __declspec(dllexport)
#else
#define LYNXENGINE_API __declspec(dllimport)
#endif
#endif

#define LYNX_VERSION_MAJOR   0
#define LYNX_VERSION_MINOR   0
#define LYNX_VERSION_RELEASE 3
#define LYNX_VERSION LYNX_VERSION_MAJOR.LYNX_VERSION_MINOR.LYNX_VERSION_RELEASE

#include "Core/ECS/components.h"
#include "Core/ECS/system.h"
#include "Core/application.h"
#include "Core/eventManager.h"
#include "Core/inputManager.h"
#include "Core/assert.h"

#include "Events/event.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"
#include "Events/joystickEvent.h"

#include "Graphics/renderer2D.h"
#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Graphics/shader.h"
#include "Graphics/mesh.h"
#include "Graphics/rendererAPI.h"
#include "Graphics/buffers.h"

#endif