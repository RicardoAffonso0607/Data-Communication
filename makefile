# Nome do executável final
EXECUTABLE := meu_programa

# Diretórios de origem
SRC_DIR := ./src
IMGUI_DIR := ./imgui
GLAD_DIR := ./glad
BUILD_DIR := ./build

# Arquivos fonte
SRCS := $(SRC_DIR)/main.cpp \
        $(GLAD_DIR)/glad.c

# Arquivos fonte do ImGui
IMGUI_SRCS := $(IMGUI_DIR)/imgui.cpp \
              $(IMGUI_DIR)/imgui_draw.cpp \
              $(IMGUI_DIR)/imgui_widgets.cpp \
              $(IMGUI_DIR)/imgui_tables.cpp \
              $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp \
              $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# Junte todos os arquivos fonte e adicione os caminhos
ALL_SRCS := $(SRCS) $(IMGUI_SRCS)

# Nome dos arquivos objeto (os .o)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(ALL_SRCS)))
OBJS += $(patsubst %.c,$(BUILD_DIR)/%.o,$(filter %.c,$(ALL_SRCS)))

# Flags do compilador
CXX := g++
CXXFLAGS := -std=c++17 -Wall -g
INCLUDES := -I$(SRC_DIR) -I$(IMGUI_DIR) -I$(GLAD_DIR)/include
LIBRARIES := -L/usr/lib -lglfw -lGL -lm -ldl
LDFLAGS := $(LIBRARIES)

# Regra principal: compilar tudo
all: $(BUILD_DIR) $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(EXECUTABLE): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

# Regra para compilar cada arquivo .o
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(BUILD_DIR)/$(EXECUTABLE)

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(BUILD_DIR)