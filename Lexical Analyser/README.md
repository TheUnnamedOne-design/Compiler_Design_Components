# Lexical Analyzer in C++

## Overview

This project implements a simple lexical analyzer in C++ that reads a source file, extracts tokens using regular expressions, and classifies them into categories such as keywords, identifiers, operators, numbers, delimiters, braces, parentheses, and string literals.

The lexical analyzer demonstrates fundamental concepts of compiler design including token extraction, pattern matching with regular expressions, and classification using hash-based data structures.

---

## Features

- Token extraction using C++ regex
- Supports multi-character operators such as <<, >>, ==, <=, >=
- Classifies tokens into:
  - Keywords
  - Identifiers
  - Operators
  - Numbers
  - Delimiters
  - Braces
  - Parentheses
  - String literals
- Reads and processes complete input source files
- Uses unordered_map and unordered_set for efficient classification

---

## Token Categories

### Keywords
Examples: int, float, long, string, using, include, return, main, cout

### Identifiers
Any valid C or C++ style identifier: variable names, function names, etc.

### Operators
Examples: +, -, *, /, =, <<, >>, ==, <=, >=

### Numbers
Integer constants such as 0, 2, 15

### Delimiters
Semicolon

### Braces and Parentheses
Braces: { }
Parentheses: ( )

### String Literals
Text enclosed within double quotes

---

## How It Works

1. The program reads all contents of the input file (for example, prog.txt).
2. The token_extractor function uses a combined regular expression to extract all tokens from the input string.
3. Each token is passed to the classifier function.
4. The classifier assigns a type based on predefined sets and checks for numbers and string literals.
5. The program outputs each unique token along with its classification.

---

## Example Output

Example for a simple input program:

```
int keyword
main identifier
( parenthesis
) parenthesis
{ brace
cout identifier
<< operator
"Hello World" string_literal
; delimiter
return keyword
} brace
```


Actual output depends on the contents of the input file.

---

## File Structure
```
|--lexical_analyser.cpp
|--prog.txt
|--README.md
```


---

## Compilation and Execution

### Compile the program:

g++ lexical_analyser.cpp -o lexical_analyser

### Run the executable:

./lexical_analyser


Ensure that prog.txt is present in the same directory or modify the file path in main().

---

## Dependencies

This project uses only standard C++ library headers:

- iostream
- fstream
- vector
- unordered_map
- unordered_set
- regex
- cstring

No external libraries are required.

---

## Limitations

- Does not handle comments (// or /* */)
- Does not detect floating point numbers
- Does not handle escape sequences within string literals
- Does not distinguish between variable identifiers and function identifiers
- Does not tokenize preprocessor directives beyond matching the keyword "include"

---

## Future Improvements

- Add support for comments
- Add support for floating point tokens
- Improve string literal handling with escape sequences
- Generate token IDs
- Build a symbol table
- Extend for use in a parser or full compiler

---
