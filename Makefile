CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lconfig++

ifeq ($(DEBUG), 1)
    CXXFLAGS += -g -O0 -DDEBUG
else
    CXXFLAGS += -O3 -DNDEBUG
endif

NAME = raytracer

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
PLUGINS_DIR = plugins

SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: prepare $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $@"
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

prepare:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/Core
	@mkdir -p $(OBJ_DIR)/Primitives
	@mkdir -p $(OBJ_DIR)/Lights
	@mkdir -p $(OBJ_DIR)/Factory
	@mkdir -p $(OBJ_DIR)/Parser
	@mkdir -p $(OBJ_DIR)/Renderer
	@mkdir -p $(PLUGINS_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	@echo "Cleaning object files"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning executable"
	@rm -f $(NAME)

re: fclean all

run: all
	@echo "Running with simple test scene"
	@./$(NAME) scenes/simple_scene.cfg

-include $(DEPS)

.PHONY: all prepare clean fclean re run