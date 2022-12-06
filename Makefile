TARGET_EXEC := demo
TARGET_ENGINE := libLynxEngine.a
BUILD_DIR := ./build
LIBS_DIR := ./deps/lib

ENGINE_SRC_DIR := src/LynxEngine
ENGINE_INC := src/LynxEngine deps/include
ENGINE_PCH := src/LynxEngine/lynx.h
ENGINE_PCH_OBJ := build/src/LynxEngine/lynx.gch
ENGINE_SRCS := $(shell find $(ENGINE_SRC_DIR) -name '*.cpp')
ENGINE_OBJS := $(ENGINE_SRCS:%=$(BUILD_DIR)/$(subst $(ENGINE_SRC_DIR),,%).o)
ENGINE_INC_FLAGS := $(addprefix -I,$(ENGINE_INC))

DEMO_SRC_DIR := src/Demo
DEMO_INC := $(ENGINE_INC)
DEMO_SRCS := $(shell find $(DEMO_SRC_DIR) -name '*.cpp')
DEMO_OBJS := $(DEMO_SRCS:%=$(BUILD_DIR)/$(subst $(DEMO_SRC_DIR),,%).o)
DEMO_INC_FLAGS := $(addprefix -I,$(ENGINE_INC))
DEMO_LINK_FLAGS := $(addprefix -L,$(LIBS_DIR)) -L./build -lassimp -lLynxEngine -lglfw3 -lGL -lGLEW -lfreetype -lX11 -lpthread -lXrandr -ldl -lz 

CPPFLAGS := $(ENGINE_INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC) : $(DEMO_OBJS) $(BUILD_DIR)/$(TARGET_ENGINE)
	$(CXX) -o $@ $(DEMO_OBJS) $(DEMO_LINK_FLAGS)

$(BUILD_DIR)/%.cpp.o : %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET_ENGINE): $(ENGINE_OBJS)
	ar rcs $@ $(ENGINE_OBJS)

$(BUILD_DIR)/%.cpp.o: %.cpp $(ENGINE_PCH_OBJ)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(ENGINE_PCH_OBJ): $(ENGINE_PCH)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean engine all
all: $(BUILD_DIR)/$(TARGET_EXEC)
engine: $(BUILD_DIR)/$(TARGET_ENGINE)
clean:
	rm -r $(BUILD_DIR)
