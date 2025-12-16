# Compiler Design – String Patterns and Validation Programs

This repository contains a focused collection of **C++ string acceptance programs** and **regular expression utilities** related to compiler design and formal language concepts.  
The programs emphasize pattern recognition, validation rules, and constraint-based string acceptance.

---

## Contents

### Regular Expression Utilities

This section includes examples of pattern matching and extraction using regular expressions, such as:
- Extracting all numeric values from a given string
- Checking whether a string starts with an uppercase letter
- Validating strong passwords containing uppercase letters, lowercase letters, digits, and special characters
- Extracting hashtags from sentences

---

### String Acceptance Programs (C++)

C++ programs that determine whether a string is accepted based on defined constraints, including:
- Strings composed of `a` and `b` that end with `b`
- Strings composed of `a` and `b` that must start with `a`
- Strings following the pattern `a⁺b⁺`
- Strings containing an equal number of `a` and `b`

---

### Advanced Pattern Constraints

Programs handling more structured and non-trivial string patterns, such as:
- Palindromic strings over the alphabet `{a, b}`
- Balanced strings of the form `aⁿbⁿcⁿ`
- Strings where all `a` characters appear before `b`, and all `b` characters appear before `c`

---

## Technologies Used

- Language: C++
- Core Concepts:
  - Regular Expressions
  - String Parsing and Validation
  - Formal Languages and Automata Theory
  - Constraint-Based Pattern Checking

---

## Build and Run Instructions

Compile any source file using a standard C++ compiler:

```bash
g++ filename.cpp -o filename
./filename
