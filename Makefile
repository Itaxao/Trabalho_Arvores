# ============================================================
# Compilador e flags
# ============================================================

CXX := g++

CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -MMD -MP
CPPFLAGS := -Ilib
BENCH_CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2


# ============================================================
# Diretórios
# ============================================================

SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build
BIN_DIR := bin


# ============================================================
# Programa principal
# ============================================================

MAIN_SRC := $(SRC_DIR)/main.cpp

# Todos os .cpp de src, exceto main.cpp
LIB_SRCS := $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.cpp))

# Objetos das estruturas
LIB_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/src/%.o,$(LIB_SRCS))

# Objeto da main
MAIN_OBJ := $(BUILD_DIR)/src/main.o

# Executável principal
APP := $(BIN_DIR)/arvores


# ============================================================
# Testes
# ============================================================

# Detecta automaticamente qualquer .cpp dentro de tests/
TEST_SRCS := $(filter-out $(TEST_DIR)/benchmark.cpp,$(wildcard $(TEST_DIR)/*.cpp))

# Exemplo:
# tests/tests_trie.cpp -> bin/tests_trie
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TEST_SRCS))


# ============================================================
# Regra padrão
# ============================================================

.PHONY: all
all: app


# ============================================================
# Programa principal
# ============================================================

.PHONY: app
app: $(APP)

$(APP): $(MAIN_OBJ) $(LIB_OBJS) | $(BIN_DIR)
	$(CXX) $(MAIN_OBJ) $(LIB_OBJS) -o $@


# ============================================================
# Compilação dos arquivos de src/
# ============================================================

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


# ============================================================
# Compilação dos testes
# ============================================================

$(BIN_DIR)/%: $(TEST_DIR)/%.cpp $(LIB_OBJS) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LIB_OBJS) -o $@


# ============================================================
# Executar programa principal
# ============================================================

.PHONY: run
run: $(APP)
	./$(APP)


# ============================================================
# Compilar todos os testes
# ============================================================

.PHONY: tests
tests: $(TEST_BINS)


# ============================================================
# Compilar e executar todos os testes
# ============================================================

.PHONY: test
test: $(TEST_BINS)
	@set -e; \
	for teste in $(TEST_BINS); do \
		echo "========================================"; \
		echo "Executando $$teste"; \
		echo "========================================"; \
		./$$teste; \
		echo ""; \
	done


# ============================================================
# Executar apenas um teste
#
# Exemplo:
# make run-tests_trie
# ============================================================

.PHONY: run-%
run-%: $(BIN_DIR)/%
	./$(BIN_DIR)/$*


# ============================================================
# Benchmark (compilado com otimização -O2)
# ============================================================

$(BIN_DIR)/benchmark: $(TEST_DIR)/benchmark.cpp $(LIB_SRCS) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(BENCH_CXXFLAGS) $< $(LIB_SRCS) -o $@

.PHONY: benchmark
benchmark: $(BIN_DIR)/benchmark
	./$(BIN_DIR)/benchmark

# ============================================================
# Geração dos gráficos a partir do CSV
# ============================================================

.PHONY: graphs
graphs:
	python3 scripts/graficos.py


# ============================================================
# Criar pasta bin
# ============================================================

$(BIN_DIR):
	mkdir -p $(BIN_DIR)


# ============================================================
# Limpeza
# ============================================================

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


# ============================================================
# Recompilar tudo
# ============================================================

.PHONY: rebuild
rebuild: clean all


# ============================================================
# Dependências automáticas dos headers
# ============================================================

DEPS := $(LIB_OBJS:.o=.d) $(MAIN_OBJ:.o=.d)

-include $(DEPS)