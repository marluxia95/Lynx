ODIR = ./src
IDIR = ./include
LDIR = ./lib
IMGUI_DIR = $(LDIR)/imgui

_OBJ = main.cpp shader.cpp texture.cpp 
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))

_LOBJ = glad/glad.c
LOBJS = $(patsubst %,$(LDIR)/%,$(_LOBJ))

IMGUI += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
IMGUI += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

_OBJ_TEST = example.cpp glad/glad.c
OBJS_TEST = $(patsubst %,$(ODIR)/%,$(_OBJ_TEST))

_DEPS = shader.h texture.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CC = g++

COMPILER_FLAGS = -w 

LINKER_FLAGS = -lglfw -lGL -lGLEW -lX11 -lpthread -lXrandr -lXi -ldl # For some reason it doesn't like -lglfw3 

OBJ_NAME = main

all : $(OBJS)
	$(CC) -I./include -I./lib $(LOBJS) $(OBJS) $(DEPS) -I./lib/imgui $(IMGUI) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -g

test : $(OBJS_TEST)
	$(CC) -I./include -I./lib $(OBJS_TEST) $(DEPS) -I./lib/imgui $(IMGUI) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)