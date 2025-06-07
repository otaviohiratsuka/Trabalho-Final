# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN = main

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Regra padrão
all: $(BIN)

# Linkagem final
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilação dos .cpp para .o no build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rodar o programa
run: $(BIN)
	./$(BIN)

# Limpar objetos e binário
clean:
	rm -rf $(BUILD_DIR) $(BIN)
