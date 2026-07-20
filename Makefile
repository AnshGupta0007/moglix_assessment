# Makefile — Longest Valid Parentheses
#
#   make            build the CLI (bin/lvp)
#   make test       build and run the unit-test suite
#   make run        build and run a couple of quick demos
#   make clean      remove build artifacts
#
# No dependencies beyond a C++17 compiler.

CXX      ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
INCLUDES  = -Iinclude

BIN_DIR   = bin
CLI       = $(BIN_DIR)/lvp
TEST_BIN  = $(BIN_DIR)/tests

.PHONY: all test run clean

all: $(CLI)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(CLI): src/main.cpp include/solution.hpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) src/main.cpp -o $(CLI)

$(TEST_BIN): tests/test_solution.cpp include/solution.hpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) tests/test_solution.cpp -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

run: $(CLI)
	@echo '"(()"    -> ' $$(./$(CLI) "(()")
	@echo '")()())" -> ' $$(./$(CLI) ")()())")
	@echo '""       -> ' $$(./$(CLI) "")

clean:
	@rm -rf $(BIN_DIR)
