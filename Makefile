BUILD_DIR = ./build

CXX=g++
AR=ar

SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(SRC:.cpp=.o)))
PCH = src/lynx.h
PCH_OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(PCH)))

DEMO_SRC = $(wildcard demo/*.cpp)
DEMO_OBJ = $(addprefix $(BUILD_DIR)/,$(notdir $(DEMO_SRC:.cpp=.o)))

CXXFLAGS = -Ideps/include -Isrc -MMD -MP
LIBFLAGS = -L./build -L./deps/lib -lassimp -llynxengine -lglfw3 -lGL -lGLEW -lfreetype -lX11 -lpthread -lXrandr -ldl -lz 

DYNAMIC_LIB = $(BUILD_DIR)/liblynxengine.so
STATIC_LIB  = $(BUILD_DIR)/liblynxengine.a
DEMO_EXE = $(BUILD_DIR)/demo

DOCXX = $(CXX) $< -c $(CXXFLAGS) -o $@

$(DEMO_EXE): $(DEMO_OBJ)
	$(CXX) -o $@ $(DEMO_OBJ) $(CXXFLAGS) $(LIBFLAGS)

$(BUILD_DIR)/%.o: demo/%.cpp | $(STATIC_LIB)
	$(DOCXX)

$(STATIC_LIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

$(BUILD_DIR)/%.o: src/%.cpp | build $(PCH_OBJ)
	$(DOCXX)

$(BUILD_DIR)/%.o: src/*/%.cpp | build $(PCH_OBJ)
	$(DOCXX)

$(PCH_OBJ): $(PCH)
	$(DOCXX)

build:
	mkdir build

.PHONY: clean dist all

all: $(STATIC_LIB) $(DEMO_EXE)

clean: dist 
	rm $(STATIC_LIB)

dist:
	rm $(OBJ) build/*.d

