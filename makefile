ODIR = .
IDIR = .
IMGUI_DIR = ./imgui

_OBJ = main.cpp shader.cpp glad/glad.c texture.cpp 
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))

IMGUI += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
IMGUI += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

_OBJ_TEST = example.cpp glad/glad.c
OBJS_TEST = $(patsubst %,$(ODIR)/%,$(_OBJ_TEST))

_DEPS = shader.h texture.h utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CC = g++

INCLUDE=-I.

COMPILER_FLAGS = -w 

LINKER_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -lXi -ldl # For some reason it doesn't like -lglfw3 

OBJ_NAME = main

all : $(OBJS)
	$(CC) $(INCLUDE) $(OBJS) $(DEPS) -I./imgui $(IMGUI) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -g

test : $(OBJS_TEST)
	$(CC) $(INCLUDE) $(OBJS_TEST) $(DEPS) -I./imgui $(IMGUI) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)