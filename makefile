ODIR = .
IDIR = .
IMGUIDIR = ./imgui

_OBJ = main.cpp shader.cpp glad/glad.c 
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ_TEST = example.cpp glad/glad.c
OBJS_TEST = $(patsubst %,$(ODIR)/%,$(_OBJ_TEST))

_DEPS = shader.h stb_image.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CC = g++

INCLUDE=-I.

COMPILER_FLAGS = -w 

LINKER_FLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl # For some reason it doesn't like -lglfw3 

OBJ_NAME = main

all : $(OBJS)
	$(CC) $(INCLUDE) $(OBJS) $(DEPS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -g

test : $(OBJS_TEST)
	$(CC) $(INCLUDE) $(OBJS_TEST) $(DEPS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)