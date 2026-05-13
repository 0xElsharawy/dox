# Dox - A Toy Compiler

**Dox** is a minimal, educational compiler that compiles a C-like language to x86-64 Intel assembly.

## Architecture

The compiler follows a traditional three-phase pipeline:

![Compiler Architecture](assets/architecture.png)

1. **Lexer** (`lexer.c`): Reads source code character-by-character and produces tokens
2. **Parser** (`parser.c`): Uses recursive descent parsing to build an Abstract Syntax Tree (AST)
3. **Code Generator** (`codegen.c`): Traverses the AST and emits x86-64 Intel syntax assembly

## Project Structure

```
├── include/          # Header files
│   ├── ast.h         # AST node definitions
│   ├── codegen.h     # Code generation interface
│   ├── lexer.h       # Lexer interface
│   ├── parser.h      # Parser interface
│   └── token.h       # Token definitions
├── src/              # Source files
│   ├── ast.c         # AST node implementation
│   ├── codegen.c     # x86-64 assembly generation
│   ├── lexer.c       # Tokenization
│   ├── main.c        # Compiler entry point
│   ├── parser.c      # Recursive descent parser
│   └── token.c       # Token utilities
├── runtime.c         # Runtime support (e.g., for `print`)
├── Makefile
└── README.md
```

## Prerequisites

- **GCC**
- **GNU Make**

## Building

```bash
make
```

This produces the compiler at `bin/dox`.

## Usage

To compile a dox source file (`.dox` extension):

```bash
./bin/dox <source.dox>
```

The compiler outputs `assembly.s` in the current directory.

## Building the Assembly

To assemble and link into an executable:

```bash
make build
# Run the compiled program
./out
```

## Supported Features

- [x] Variable declarations with optional initialization (only `int` type)
- [x] Arithmetic expressions with operator precedence
- [x] Unary operators (negation, logical NOT, bitwise NOT)
- [x] Print statements for output (e.g., `print(x)`)
- [x] Return statements
- [x] Conditional statements (`if`, `else`)
- [x] Loops (`while`)
- [ ] Functions

<p align="center">
    <strong>If you liked this project, consider giving it a ⭐.</strong>
</p>
