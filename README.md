# SimpleGameEngine
Just a simple game engine made in C++ and OpenGL

DISCLAIMER : THIS IS A WIP PROJECT, which means that its NOT finished yet!


## Building

### Linux

#### Installing dependencies

##### Installing GLFW3
You can either build it or install it via APT as it is shown here:
```bash 
sudo apt install libglfw3 libglfw3-dev
```

##### Installing GLAD

Download GLAD from [here](https://glad.dav1d.de/), then select the latest GL version and click on `generate`. Then unzip it into the `lib` folder so there is a `glad` and `KHR` folder.

##### Intalling GLM

Download the last version from GLM  [here](https://github.com/g-truc/glm/tags) then extract it into the `lib` folder so there's a `glm` folder inside it.

### Compiling

In order to compile, you just type `make` and it should generate a binary file named `main`.




