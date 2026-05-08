CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TEST_SCRIPT= tests/run_tests.sh

TARGET = $(BIN_DIR)/tinycc

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(BUILD_DIR):
	@mkdir -p $@

test: $(TARGET)
	@chmod +x $(TEST_SCRIPT)
	@./$(TEST_SCRIPT)

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
