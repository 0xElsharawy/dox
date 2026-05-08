CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude -D_DEFAULT_SOURCE

SRC_DIR = src
BIN_DIR = bin
TEST_SCRIPT = tests/run_tests.sh

TARGET = $(BIN_DIR)/tinycc

# Find all .c files in src/
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Default target
all: $(TARGET)

# Compile all source files directly into the executable
$(TARGET): $(SRCS) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	@mkdir -p $@

# Run tests
test: $(TARGET)
	@chmod +x $(TEST_SCRIPT)
	@./$(TEST_SCRIPT)

# Clean only the bin directory
clean:
	@rm -rf $(BIN_DIR)

.PHONY: all clean test
