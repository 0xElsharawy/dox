CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude -D_DEFAULT_SOURCE

SRC_DIR = src
BIN_DIR = bin
ASSEMBLY_FILE = assembly.s
RUNTIME_FILE = runtime.c
MACHINE_FILE = out

TARGET = $(BIN_DIR)/dox

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

# Compile assembly code to machine code
build:
	@$(CC) -no-pie $(ASSEMBLY_FILE) $(RUNTIME_FILE) -o $(MACHINE_FILE)

# Clean and remove generated files
clean:
	@rm -rf $(BIN_DIR) $(ASSEMBLY_FILE) $(MACHINE_FILE)

.PHONY: all clean build
