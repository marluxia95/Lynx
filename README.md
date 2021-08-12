# SimpleGameEngine 
## OpenGL-Only branch
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/43d567336363474da3830f0cd6d01728)](https://www.codacy.com/gh/marluxia95/SimpleGameEngine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=marluxia95/SimpleGameEngine&amp;utm_campaign=Badge_Grade)\
Just a simple game engine made in C++ and OpenGL. This branch has no rendererAPI

Link model is from here `https://www.models-resource.com/nintendo_64/`


## Dependencies
This project requires the following libraries :
```
GLFW3
GLEW
GLM
Assimp
```

## Building

### Linux

#### Installing Dependencies

You can either build it or install it via APT as it is shown here:
```bash 
sudo apt install libglfw3-dev libglew-dev libassimp-dev
```

#### Building
In order to build the project, run `chmod +x build_LINUX.sh` and `./build_LINUX.sh`, it will automatically download and check libraries. You can also build it by running `cmake ..` and `make` in the `Build` folder.

### Windows
 
Current branch has no support for windows, yet.
