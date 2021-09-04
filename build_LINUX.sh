#!/bin/bash


echo "Checking if GLM is installed..."
if [ -d "./lib/glm" ]; then
    echo "GLM already installed"
else
    cd lib
    echo "Downloading GLM"
    wget "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip"
    unzip glm-0.9.9.8.zip
    cd glm 
    cp -R ./* ..
    cd ../..
fi

echo "Checking if imgui is installed..."
if [ -d "./lib/imgui" ]; then
    echo "ImGui already installed"
else
    cd lib
    echo "Downloading ImGUI"
    git clone https://github.com/ocornut/imgui
    cd imgui/backends
    cp imgui_impl_glfw.cpp ../imgui_impl_glfw.cpp
    cp imgui_impl_glfw.h ../imgui_impl_glfw.h
    cp imgui_impl_opengl3.cpp ../imgui_impl_opengl3.cpp
    cp imgui_impl_opengl3.h ../imgui_impl_opengl3.h
    cd ..
fi

if [ -f "./lib/stb_image.h" ]; then
    echo "stb_image already installed"
else
    cd lib
    echo "Downloading stb_image"
    wget "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
    cd ..
fi

echo "Downloaded all dependencies"
