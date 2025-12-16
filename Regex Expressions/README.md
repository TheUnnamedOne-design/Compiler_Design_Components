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

---

I. REGULAR EXPRESSIONS 

1. Write a regex to extract all numbers from a given string. 

2. Write a regex to check if a string starts with an uppercase letter. 

3. Write a regex to validate a strong password containing uppercase, lowercase, digits, and a special character. 

4. Write a regex to extract all hashtags from a sentence. 

II. C++ STRING ACCEPTANCE PROGRAMS 

5. Write a C++ program to check whether the string is accepted or not – the string comprises zero or more occurrences of a and b and ends with b. 

6. Write a C++ program to check whether the string is accepted or not – the string contains only a and b and must start with a. 

7. Write a C++ program to check whether the string is accepted or not – the string contains one or more a’s followed by one or more b’s (pattern: a⁺b⁺). 

8. Write a C++ program to check whether the string is accepted or not – the string contains equal number of a’s and b’s. 

III. ADVANCED STRING PATTERN PROGRAMS 

9. Palindromic String Check (over {a, b}) 

Write a C++ program to check whether the string is accepted or not – the string must be a palindrome and should contain only a and b. 

10. Balanced Pattern aⁿbⁿcⁿ (Equal Count Constraint) 

Write a C++ program to check whether the string is accepted or not – the string must contain equal numbers of a’s, b’s, and c’s in sequence (pattern aⁿbⁿcⁿ, where n ≥ 1). 

11. String Accepted Only if a Appears Before b and b Appears Before c 

Write a C++ program to check whether the string is accepted or not – the string can have any number of a’s, followed by any number of b’s, followed by any number of c’s, but no other order is allowed. 