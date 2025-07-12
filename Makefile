# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -flto -fopenmp -Iinclude
LDFLAGS = -fopenmp

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN = main

# Busca recursiva por .cpp dentro de src/
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
# Gera os .o equivalentes no build/ com mesmo nome relativo
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Regra padrão
all: $(BIN)

# Linkagem final
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compilação com criação de subpastas em build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rodar o programa
run: $(BIN)
	./$(BIN)

# Limpar objetos e binário
clean:
	rm -rf $(BUILD_DIR) $(BIN)

# Regra para compilar sem OpenMP (caso necessário)
no-openmp: CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -flto -Iinclude
no-openmp: LDFLAGS = 
no-openmp: $(BIN)

.PHONY: all run clean no-openmp