# SimpleGameEngine
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/43d567336363474da3830f0cd6d01728)](https://www.codacy.com/gh/marluxia95/SimpleGameEngine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=marluxia95/SimpleGameEngine&amp;utm_campaign=Badge_Grade)\
Just a simple game engine made in C++ and OpenGL

DISCLAIMER : THIS IS A WIP PROJECT, which means that its NOT finished yet!

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

Download the last version from GLM  [here](https://github.com/g-truc/glm/tags) then extract it into the `lib` folder leaving a `glm` folder inside it.

#### Building
In order to build the project, run `chmod +x build_LINUX.sh` and `./build_LINUX.sh`, it will automatically download and check libraries. You can also build it by running `cmake ..` and `make` in the `Build` folder.

### Windows

#### Installing Dependencies 
Make sure you have installed Visual Studio ( 2019 recommended but might also work with 2017 ).

Download the last version from GLM  [here](https://github.com/g-truc/glm/tags) then extract it into the `lib` folder leaving a `glm` folder inside it.

Download the GLEW binaries [here](https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download) and copy the library files from `lib/Release/x64` and paste them in the `lib` folder.
Copy the GLEW binary `.dll` and paste it into the main project folder.

Download the GLFW binary files from [here](https://github.com/glfw/glfw/releases/tag/3.3.4) and copy the `.lib` files from `lib-vc20XX` into the `lib` folder and the `.dll` files into the main project folder.

Clone the ASSIMP library from [here](https://github.com/assimp/assimp/releases/tag/v4.1.0) and copy the `include/assimp` folder into the `lib` folder. You have to also compile it in order to get the `config.h` file. Once compiled ASSIMP, paste the `config.h` file into the `lib/assimp` folder.

#### Building

In order to build the project, open the `.sln` file in Visual Studio and make sure you have set any of the `x64` platforms. Then press the build button and it should generate the binaries either in the `Debug` folder or the `Release` folder, depending on what platform you've chosen.

