CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lconfig++

# Plugin compilation flags
PLUGIN_CXXFLAGS = $(CXXFLAGS) -fPIC
PLUGIN_LDFLAGS = -shared

ifeq ($(DEBUG), 1)
    CXXFLAGS += -g -O0 -DDEBUG
    PLUGIN_CXXFLAGS += -g -O0 -DDEBUG
else
    CXXFLAGS += -O3 -DNDEBUG
    PLUGIN_CXXFLAGS += -O3 -DNDEBUG
endif

NAME = raytracer

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
PLUGINS_DIR = plugins
BUILD_PLUGINS_DIR = build/plugins

# Source files for main program
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Plugin source files
PLUGIN_SRCS = $(shell find $(PLUGINS_DIR) -name "*.cpp" 2>/dev/null || echo "")
PLUGIN_OBJS = $(PLUGIN_SRCS:$(PLUGINS_DIR)/%.cpp=$(BUILD_PLUGINS_DIR)/%.so)

# Special object files for plugins (compiled with -fPIC)
PLUGIN_OBJ_DIR = $(OBJ_DIR)/plugins
PLUGIN_CORE_SRCS = src/Core/Vector3D.cpp src/Core/Material.cpp src/Core/Color.cpp src/Core/Ray.cpp
PLUGIN_CORE_OBJS = $(PLUGIN_CORE_SRCS:src/%.cpp=$(PLUGIN_OBJ_DIR)/%.o)

all: prepare $(NAME) plugins

$(NAME): $(OBJS)
	@echo "Linking $@"
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

plugins: $(PLUGIN_OBJS)

# Rule for building plugins
$(BUILD_PLUGINS_DIR)/%.so: $(PLUGINS_DIR)/%.cpp $(PLUGIN_CORE_OBJS)
	@echo "Building plugin $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(PLUGIN_CXXFLAGS) $(PLUGIN_LDFLAGS) $< $(PLUGIN_CORE_OBJS) -o $@

# Special rule for plugin core objects (compiled with -fPIC)
$(PLUGIN_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $< for plugins"
	@mkdir -p $(dir $@)
	@$(CXX) $(PLUGIN_CXXFLAGS) -MMD -MP -c $< -o $@

prepare:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/Core
	@mkdir -p $(OBJ_DIR)/Primitives
	@mkdir -p $(OBJ_DIR)/Lights
	@mkdir -p $(OBJ_DIR)/Factory
	@mkdir -p $(OBJ_DIR)/Parser
	@mkdir -p $(OBJ_DIR)/Renderer
	@mkdir -p $(PLUGIN_OBJ_DIR)
	@mkdir -p $(PLUGIN_OBJ_DIR)/Core
	@mkdir -p $(BUILD_PLUGINS_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	@echo "Cleaning object files"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning executable and plugins"
	@rm -f $(NAME)
	@rm -rf $(BUILD_PLUGINS_DIR)

re: fclean all

# Clean only plugins
clean-plugins:
	@echo "Cleaning plugins"
	@rm -rf $(BUILD_PLUGINS_DIR)
	@rm -rf $(PLUGIN_OBJ_DIR)

run: all
	@echo "Running with simple test scene"
	@./$(NAME) scenes/simple_scene.cfg

# Test avec plugins
run-plugins: all
	@echo "Running with plugin test scene"
	@./$(NAME) scenes/plugin_test.cfg

-include $(DEPS)

.PHONY: all prepare clean fclean re run run-plugins plugins clean-plugins