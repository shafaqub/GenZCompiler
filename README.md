# AERIS – GenZ Compiler

AERIS is a custom compiler project developed to demonstrate the fundamental phases of compiler construction. It is designed around a programming language with its own keywords, data types, operators, and syntax.

## Project Overview

The compiler is being developed in C++ and follows a modular architecture. Its development includes lexical analysis, syntax analysis, abstract syntax tree generation, and semantic analysis.

## Current Implementation

### 1. Lexical Analysis

The lexical analyzer reads source code and converts it into a sequence of tokens.

Implemented features include:
- Keyword and identifier recognition.
- Whole number, decimal, string, and character literal recognition.
- Arithmetic, relational, equality, assignment, initialization, and logical operator recognition.
- Delimiter and comment handling.
- Symbol table generation.
- Token stream generation.
- End-of-file token recognition.

### 2. Lexical Error Handling

The compiler detects and reports lexical errors, including:
- Invalid characters.
- Invalid identifiers.
- Malformed numbers.
- Unclosed string literals.
- Invalid character literals.
- Invalid operators.

Error reports include line numbers, column positions, source code context, and caret-based error highlighting.

## Project Structure

```text
AERIS/
├── src/
│   ├── lexer/
│   │   ├── lexer.cpp
│   │   ├── lexer.h
│   │   └── lexer_runner.cpp
│   ├── errors/
│   │   ├── error_handler.cpp
│   │   └── error_handler.h
│   └── main.cpp
├── tests/
├── backup/
├── input_code.txt
├── tokens.txt
├── symbol_table.txt
└── lexical_errors.txt
```

## Build and Run

Compile the project using:

```bash
g++ -std=c++11 -Isrc src/main.cpp src/lexer/lexer.cpp src/lexer/lexer_runner.cpp src/errors/error_handler.cpp -o build/compiler.exe
```

Run the compiler:

```bash
./build/compiler.exe
```

On Windows PowerShell:

```powershell
.\build\compiler.exe
```

## Output Files

The compiler generates:

- `tokens.txt` – Contains the generated token stream.
- `symbol_table.txt` – Contains information about recognized identifiers.
- `lexical_errors.txt` – Contains detected lexical errors and their locations.

## Development Status

Lexical analysis and lexical error handling have been implemented and tested. Further compiler phases, including parsing, AST generation, and semantic analysis, are planned for subsequent development.

## Technologies Used

- **Programming Language:** C++
- **Compiler Standard:** C++11
- **Version Control:** Git and GitHub