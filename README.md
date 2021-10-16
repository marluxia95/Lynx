# LynxEngine 
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/43d567336363474da3830f0cd6d01728)](https://www.codacy.com/gh/marluxia95/SimpleGameEngine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=marluxia95/SimpleGameEngine&amp;utm_campaign=Badge_Grade)\
Just a simple game engine made in C++ and OpenGL. 

Link model is from here `https://www.models-resource.com/nintendo_64/`

NOTE: Current version in this repository is unstable and will probably not compile properly, please download the latest stable release from the `Releases` tab. 
## Features

- Model Loading
- Cross-platform ( different API support, for now there's only OpenGL )
- ECS
- Lighting 
- Custom shader parsing
- Skyboxes
- Pretty organized input system with joystick support 
- Logging and debug features
- Fully event-driven
- Fully documented ( Via doxygen )

## TODO

- Terrain rendering 
- Physics engine
- Sound system
- Batch rendering ( with dynamic VBOs )
- Better resource loading
- LUA Scripting
- Multithreading

## Dependencies
This project requires the following libraries :
```
GLFW3
GLEW
GLM
Assimp
```

However, it is not necessary to install them, since it will download and build them automatically.

## Building

### Linux

Create a build folder then run `cmake ..` then run `make install` or `make -j8 install` if you want to use more than 1 thread for building.
NOTE : `make install` won't install the program into your system, it will just create the necessary folders inside the `build` folder.

### Windows

Building on windows does not work at the moment. I will fix it as soon as possible.

To build the project on Windows, you need cmake and visual studio ( 2019 recommended ).

Create a build folder then run `cmake ..`, it will generate a visual studio solution file, then open the `INSTALL` project with visual studio and press on build. 