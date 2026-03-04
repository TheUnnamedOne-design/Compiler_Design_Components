# CLR Parser

## Description
Implementation of a **Canonical LR (CLR) Parser** for the given grammar:
- S → CC
- C → cC
- C → d

## Grammar
```
S -> CC
C -> cC
C -> d
```

## Input String Tested
**Input:** `ccdd`  
**Result:** ✅ ACCEPTED

## Key Features

### 1. LR(1) Items with Lookaheads
- CLR parser uses LR(1) items: `[A → α.β, a]`
- The lookahead symbol 'a' provides precise parsing decisions
- More powerful than SLR - resolves shift/reduce conflicts that SLR cannot

### 2. Complete Implementation Components
- **Grammar Structure**: Stores production rules
- **LR(1) Items**: Items with lookahead symbols for CLR parsing
- **FIRST Sets**: Computed for lookahead propagation
- **Closure Operation**: Expands items with lookahead calculation
- **GOTO Function**: Computes state transitions
- **LR(1) States**: Canonical collection of LR(1) items
- **CLR Parsing Table**: ACTION and GOTO tables with lookahead-specific actions
- **Parser**: Stack-based parser with detailed step-by-step trace

### 3. CLR vs SLR Differences

| Feature | SLR | CLR |
|---------|-----|-----|
| Items | LR(0) items: `[A → α.β]` | LR(1) items: `[A → α.β, a]` |
| Reduce Action | Uses FOLLOW set (all symbols) | Uses specific lookahead symbol |
| Parsing Power | Less powerful | More powerful |
| States | Fewer states | More states |
| Conflicts | More conflicts possible | Fewer conflicts |

### 4. Lookahead Propagation (CARRY Logic)
When computing closure for item `[A → α.Bβ, a]`:
- For each production `B → γ`
- Calculate FIRST(βa)
- Add items `[B → .γ, b]` for each `b` in FIRST(βa)
- This "carries" the lookahead information forward

## Output Components

### 1. Grammar Productions
Shows all production rules including augmented start production

### 2. FIRST Sets
Displays FIRST sets for non-terminals used in lookahead calculation

### 3. LR(1) State Generation
Shows the construction of each state with GOTO transitions

### 4. LR(1) Canonical Collection
Displays all states with their LR(1) items (including lookaheads)

### 5. CLR Parsing Table
- **ACTION Table**: Shift, Reduce, and Accept actions based on lookaheads
- **GOTO Table**: State transitions for non-terminals

### 6. Parsing Trace
Step-by-step parsing showing:
- Stack contents (symbols and states)
- Remaining input
- Action taken at each step

## Sample Output Structure

```
Grammar Productions:
===================
C -> cC
C -> d
S -> CC
X -> S

FIRST Sets:
===========
FIRST(S) = { c, d }
FIRST(C) = { c, d }

LR(1) Canonical Collection of Items:
====================================
I0:
  [C -> .cC, c]
  [C -> .cC, d]
  [C -> .d, c]
  [C -> .d, d]
  [S -> .CC, $]
  [X -> .S, $]
...

CLR Parsing Table:
==================
(Shows ACTION and GOTO tables)

Parsing Input String: ccdd
================================================
(Shows step-by-step parsing trace)
```

## How to Compile and Run

### Using g++:
```bash
g++ CLR.cpp -o CLR.exe
./CLR.exe
```

### Using VS Code:
- Open CLR.cpp
- Click "Run Code" or press Ctrl+Alt+N

## Technical Details

### Augmented Grammar
The parser augments the original grammar with:
```
X -> S
```
This allows proper acceptance detection when X → S. is complete with lookahead $.

### Parsing Algorithm
1. Start with state 0 and input string + $
2. Repeat:
   - Read current state and input symbol
   - Look up ACTION table
   - If SHIFT: Push symbol and new state
   - If REDUCE: Pop RHS symbols, push LHS, use GOTO
   - If ACCEPT: String is valid
   - If ERROR: String is invalid

### State Structure
Each state contains a set of LR(1) items where each item has:
- Left-hand side non-terminal
- Right-hand side production
- Dot position
- **Lookahead symbol** (KEY difference from LR(0))

## Submission Information
- **Due Date**: March 4, 2026
- **Status**: ✅ Complete
- **Test String**: ccdd (ACCEPTED)

## Why CLR is Better than SLR
The CLR parser uses specific lookahead symbols for reduce actions, while SLR uses the entire FOLLOW set. This makes CLR more precise and able to parse more grammars without conflicts.

**Example**: For item `[C → d., c]`, CLR only reduces on 'c', while SLR would reduce on all symbols in FOLLOW(C), which might include additional symbols that should trigger shifts instead.
