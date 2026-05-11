# TinyCC - A Toy C Compiler

**TinyCC (Tiny C Compiler)** is minimal, educational C compiler that compiles a subset of C to x86-64 Intel assembly.

## Features

- **Integer type** - Only `int` is supported
- **Function declarations** - Single function compilation
- **Local variables** - Stored on the stack
- **Arithmetic expressions** - Addition, subtraction, multiplication, division
- **Unary operators** - Negation (`-`), logical NOT (`!`), bitwise NOT (`~`)
- **Assignment** - Variable assignment with optional initializer
- **Return statements** - Function return values

## Project Structure

```
tinycc/
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
├── Makefile
└── README.md
```

## Architecture

The compiler follows a traditional three-phase pipeline:

```
Source Code → Lexer → Tokens → Parser → AST → Code Generator → Assembly
```

1. **Lexer** (`lexer.c`): Reads source code character-by-character and produces tokens
2. **Parser** (`parser.c`): Uses recursive descent parsing to build an Abstract Syntax Tree (AST)
3. **Code Generator** (`codegen.c`): Traverses the AST and emits x86-64 Intel syntax assembly

## Prerequisites

- **GCC**
- **GNU Make**

## Building

```bash
make
```

This produces the compiler at `bin/tinycc`.

## Usage

```bash
./bin/tinycc <source_file.c>
```

The compiler outputs `assembly.s` in the current directory.

## Example

Input (`test.c`):

```c
int main() {
    int x = 1;
    x = x + x;
    return (3 + x) * x;
}
```

Output (`assembly.s`):

```asm
.intel_syntax noprefix
.global main

main:
    push rbp
    mov rbp, rsp
    sub rsp, 256
    mov rax, 1
    mov [rbp-8], rax
    mov rax, [rbp-8]
    push rax
    mov rax, [rbp-8]
    mov rbx, rax
    pop rax
    add rax, rbx
    mov [rbp-8], rax
    mov rax, 3
    push rax
    mov rax, [rbp-8]
    mov rbx, rax
    pop rax
    add rax, rbx
    push rax
    mov rax, [rbp-8]
    mov rbx, rax
    pop rax
    imul rax, rbx
    mov rsp, rbp
    pop rbp
    ret
```

## Building the Assembly

To assemble and link into an executable:

```bash
gcc -no-pie assembly.s -o out
./out
# Should print the return value of main (e.g. 10)
echo $?
```

## Supported Syntax

| Category   | Syntax                 |
| ---------- | ---------------------- |
| Variables  | `int x;`, `int x = 5;` |
| Assignment | `x = 5 + 3;`           |
| Arithmetic | `+`, `-`, `*`, `/`     |
| Unary      | `-x`, `!x`, `~x`       |
| Control    | `return <expr>;`       |
| Blocks     | `{ ... }`              |

## Limitations

- Single function compilation only
- Integer type only (no floats, no strings)
- No function calls, no loops, no conditionals
- No global variables
- No preprocessor directives
- No arrays or structs

This is an educational project for learning compiler construction.
