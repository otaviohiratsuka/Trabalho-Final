# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -flto -Iinclude 
LDFLAGS = 

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

# Regra para compilar com OpenMP (caso necessário para comparação)
with-openmp: CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -flto -fopenmp -Iinclude
with-openmp: LDFLAGS = -fopenmp
with-openmp: $(BIN)

# Regra para compilar para debug
debug: CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0 -Iinclude
debug: LDFLAGS = 
debug: $(BIN)

# Regra para limpeza de arquivos temporários gerados pelo algoritmo
clean-temp:
	rm -f temp_*.dat
	rm -f outcome/output_*.dat
	rm -f datasets/explore_test.dat

# Regra para limpeza completa (inclui arquivos temporários)
clean-all: clean clean-temp

.PHONY: all run clean with-openmp debug clean-temp clean-all