#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

// Grammar structure
struct Grammar {
    map<char, vector<string>> rules;

    void print() const {
        cout << "Grammar Productions:\n";
        cout << "===================\n";
        for (auto &p : rules) {
            for (auto &rhs : p.second) {
                cout << p.first << " -> " << rhs << endl;
            }
        }
        cout << endl;
    }
};

// LR(1) Item: Includes lookahead symbol
struct LR1Item {
    char lhs;
    string rhs;
    int dot;
    char lookahead;  // Lookahead symbol for CLR

    bool operator==(const LR1Item& other) const {
        return lhs == other.lhs &&
               rhs == other.rhs &&
               dot == other.dot &&
               lookahead == other.lookahead;
    }

    bool operator<(const LR1Item& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        if (dot != other.dot) return dot < other.dot;
        return lookahead < other.lookahead;
    }

    void print() const {
        cout << "  [" << lhs << " -> ";
        for (int i = 0; i < rhs.size(); i++) {
            if (i == dot)
                cout << ".";
            cout << rhs[i];
        }
        if (dot == rhs.size())
            cout << ".";
        cout << ", " << lookahead << "]" << endl;
    }
};

// Hash function for LR(1) items
struct LR1ItemHash {
    size_t operator()(const LR1Item& item) const {
        size_t h = hash<char>()(item.lhs);
        for (char c : item.rhs)
            h ^= hash<char>()(c) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= hash<int>()(item.dot);
        h ^= hash<char>()(item.lookahead);
        return h;
    }
};

// State containing LR(1) items
struct State {
    set<LR1Item> items;

    bool operator==(const State& other) const {
        return items == other.items;
    }

    bool operator<(const State& other) const {
        return items < other.items;
    }

    void print() const {
        for (const auto& item : items) {
            item.print();
        }
    }
};

// Hash function for states
struct StateHash {
    size_t operator()(const State& state) const {
        size_t h = 0;
        LR1ItemHash item_hash;
        for (const auto& item : state.items) {
            h ^= item_hash(item);
        }
        return h;
    }
};

// Manages states and their numbers
class StatesManager {
public:
    unordered_map<State, int, StateHash> stateMap;

    void add(State s, int state_num) {
        stateMap[s] = state_num;
    }

    bool find(State s) {
        return stateMap.find(s) != stateMap.end();
    }

    int getStateNum(State s) {
        return stateMap[s];
    }

    void print() {
        vector<pair<int, State>> sorted_states;
        for (auto& it : stateMap) {
            sorted_states.push_back({it.second, it.first});
        }
        sort(sorted_states.begin(), sorted_states.end());

        cout << "LR(1) Canonical Collection of Items:\n";
        cout << "====================================\n\n";

        for (auto& p : sorted_states) {
            cout << "I" << p.first << ":\n";
            p.second.print();
            cout << endl;
        }
    }
};

StatesManager states;
int stateCounter = 0;

// Compute FIRST sets
map<char, unordered_set<char>> computeFirst(const Grammar& grammar, const unordered_set<char>& non_terminals) {
    map<char, unordered_set<char>> first;
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& rule : grammar.rules) {
            char lhs = rule.first;
            
            for (const auto& rhs : rule.second) {
                if (rhs.empty() || rhs == "e") {
                    if (first[lhs].insert('e').second) {
                        changed = true;
                    }
                    continue;
                }
                
                char firstSymbol = rhs[0];
                
                if (non_terminals.find(firstSymbol) == non_terminals.end()) {
                    if (first[lhs].insert(firstSymbol).second) {
                        changed = true;
                    }
                } else {
                    size_t oldSize = first[lhs].size();
                    for (char c : first[firstSymbol]) {
                        first[lhs].insert(c);
                    }
                    if (first[lhs].size() > oldSize) {
                        changed = true;
                    }
                }
            }
        }
    }
    
    return first;
}

// Compute FIRST of a string (for lookahead calculation)
unordered_set<char> computeFirstOfString(const string& str, 
                                         const unordered_set<char>& terminals,
                                         const unordered_set<char>& non_terminals,
                                         const map<char, unordered_set<char>>& first) {
    unordered_set<char> result;
    
    if (str.empty()) {
        result.insert('e');
        return result;
    }
    
    char firstChar = str[0];
    
    if (terminals.find(firstChar) != terminals.end()) {
        result.insert(firstChar);
    } else if (non_terminals.find(firstChar) != non_terminals.end()) {
        if (first.count(firstChar) > 0) {
            for (char c : first.at(firstChar)) {
                result.insert(c);
            }
        }
    }
    
    return result;
}

// Compute closure for LR(1) items with lookahead propagation
void computeClosure(State* state, const Grammar& grammar, 
                   const unordered_set<char>& terminals,
                   const unordered_set<char>& non_terminals,
                   const map<char, unordered_set<char>>& first) {
    bool changed = true;
    while (changed) {
        changed = false;
        set<LR1Item> newItems;

        for (const auto& item : state->items) {
            if (item.dot < item.rhs.size()) {
                char nextSymbol = item.rhs[item.dot];
                
                // If next symbol is a non-terminal, add closure items
                if (non_terminals.find(nextSymbol) != non_terminals.end()) {
                    if (grammar.rules.find(nextSymbol) != grammar.rules.end()) {
                        
                        // Calculate lookahead for new items
                        string beta = "";
                        if (item.dot + 1 < item.rhs.size()) {
                            beta += item.rhs[item.dot + 1];
                        }
                        
                        unordered_set<char> lookaheads;
                        if (!beta.empty()) {
                            lookaheads = computeFirstOfString(beta, terminals, non_terminals, first);
                            // If beta can derive epsilon, add item's lookahead
                            if (lookaheads.count('e') > 0) {
                                lookaheads.erase('e');
                                lookaheads.insert(item.lookahead);
                            }
                        } else {
                            lookaheads.insert(item.lookahead);
                        }
                        
                        // Add new items for each production of the non-terminal
                        for (const auto& production : grammar.rules.at(nextSymbol)) {
                            for (char la : lookaheads) {
                                LR1Item newItem;
                                newItem.lhs = nextSymbol;
                                newItem.rhs = production;
                                newItem.dot = 0;
                                newItem.lookahead = la;
                                
                                if (state->items.find(newItem) == state->items.end()) {
                                    newItems.insert(newItem);
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        for (const auto& item : newItems) {
            state->items.insert(item);
        }
    }
}

// Compute GOTO for a state and symbol
State computeGoto(const State& state, char symbol, const Grammar& grammar,
                 const unordered_set<char>& terminals,
                 const unordered_set<char>& non_terminals,
                 const map<char, unordered_set<char>>& first) {
    State newState;

    for (const auto& item : state.items) {
        if (item.dot < item.rhs.size() && item.rhs[item.dot] == symbol) {
            LR1Item newItem;
            newItem.lhs = item.lhs;
            newItem.rhs = item.rhs;
            newItem.dot = item.dot + 1;
            newItem.lookahead = item.lookahead;
            
            newState.items.insert(newItem);
        }
    }

    if (!newState.items.empty()) {
        computeClosure(&newState, grammar, terminals, non_terminals, first);
    }

    return newState;
}

// Get all symbols after the dot in current state
unordered_set<char> getSymbols(const State& state) {
    unordered_set<char> symbols;
    for (const auto& item : state.items) {
        if (item.dot < item.rhs.size()) {
            symbols.insert(item.rhs[item.dot]);
        }
    }
    return symbols;
}

// Action structure for parsing table
struct Action {
    string type;
    int state;
    char lhs;
    string rhs;
    
    Action() : type("ERROR"), state(-1), lhs('\0'), rhs("") {}
};

// Build CLR parsing table
void buildCLRParsingTable(map<int, map<char, Action>>& actionTable,
                          map<int, map<char, int>>& gotoTable,
                          const Grammar& grammar,
                          const StatesManager& states,
                          const unordered_set<char>& terminals,
                          const unordered_set<char>& non_terminals,
                          const map<char, unordered_set<char>>& first) {
    
    for (const auto& statePair : states.stateMap) {
        const State& state = statePair.first;
        int stateNum = statePair.second;
        
        for (const auto& item : state.items) {
            // Reduce items (dot at the end)
            if (item.dot >= item.rhs.size()) {
                if (item.lhs == 'X' && item.rhs == "S") {
                    // Accept action
                    actionTable[stateNum]['$'].type = "ACCEPT";
                } else {
                    // Reduce action only on the specific lookahead
                    actionTable[stateNum][item.lookahead].type = "REDUCE";
                    actionTable[stateNum][item.lookahead].lhs = item.lhs;
                    actionTable[stateNum][item.lookahead].rhs = item.rhs;
                }
            }
        }
        
        // Shift and Goto entries
        for (const auto& item : state.items) {
            if (item.dot < item.rhs.size()) {
                char nextSymbol = item.rhs[item.dot];
                
                State nextState = computeGoto(state, nextSymbol, grammar, terminals, non_terminals, first);
                if (!nextState.items.empty() && states.stateMap.count(nextState) > 0) {
                    int nextStateNum = states.stateMap.at(nextState);
                    
                    if (terminals.find(nextSymbol) != terminals.end()) {
                        // Shift action
                        actionTable[stateNum][nextSymbol].type = "SHIFT";
                        actionTable[stateNum][nextSymbol].state = nextStateNum;
                    } else if (non_terminals.find(nextSymbol) != non_terminals.end()) {
                        // Goto entry
                        gotoTable[stateNum][nextSymbol] = nextStateNum;
                    }
                }
            }
        }
    }
}

// Print CLR parsing table
void printCLRParsingTable(const map<int, map<char, Action>>& actionTable,
                          const map<int, map<char, int>>& gotoTable,
                          const unordered_set<char>& terminals,
                          const unordered_set<char>& non_terminals,
                          int numStates) {
    
    cout << "CLR Parsing Table:\n";
    cout << "==================\n\n";
    
    vector<char> terminalsList(terminals.begin(), terminals.end());
    sort(terminalsList.begin(), terminalsList.end());
    
    vector<char> nonTerminalsList;
    for (char nt : non_terminals) {
        if (nt != 'X') {
            nonTerminalsList.push_back(nt);
        }
    }
    sort(nonTerminalsList.begin(), nonTerminalsList.end());
    
    // Print header
    cout << "State\t";
    cout << "ACTION";
    for (size_t i = 1; i < terminalsList.size(); i++) {
        cout << "\t";
    }
    cout << "\t\t|\tGOTO\n";
    
    cout << "\t";
    for (char t : terminalsList) {
        cout << t << "\t";
    }
    cout << "|\t";
    for (char nt : nonTerminalsList) {
        cout << nt << "\t";
    }
    cout << endl;
    
    cout << "-----\t";
    for (size_t i = 0; i < terminalsList.size(); i++) {
        cout << "-----\t";
    }
    cout << "|\t";
    for (size_t i = 0; i < nonTerminalsList.size(); i++) {
        cout << "-----\t";
    }
    cout << endl;
    
    // Print table rows
    for (int i = 0; i < numStates; i++) {
        cout << i << "\t";
        
        // Action columns
        for (char t : terminalsList) {
            if (actionTable.count(i) > 0 && actionTable.at(i).count(t) > 0) {
                const Action& action = actionTable.at(i).at(t);
                if (action.type == "SHIFT") {
                    cout << "s" << action.state << "\t";
                } else if (action.type == "REDUCE") {
                    cout << "r" << action.lhs << "->" << action.rhs << "\t";
                } else if (action.type == "ACCEPT") {
                    cout << "acc\t";
                }
            } else {
                cout << "\t";
            }
        }
        
        cout << "|\t";
        
        // Goto columns
        for (char nt : nonTerminalsList) {
            if (gotoTable.count(i) > 0 && gotoTable.at(i).count(nt) > 0) {
                cout << gotoTable.at(i).at(nt) << "\t";
            } else {
                cout << "\t";
            }
        }
        
        cout << endl;
    }
    cout << endl;
}

// Parse input string using CLR parsing table
void parseInput(string input, 
                const map<int, map<char, Action>>& actionTable,
                const map<int, map<char, int>>& gotoTable,
                const Grammar& grammar) {
    
    cout << "Parsing Input String: " << input << endl;
    cout << "================================================\n\n";
    
    vector<int> stateStack;
    vector<char> symbolStack;
    
    stateStack.push_back(0);
    symbolStack.push_back('#');
    
    input += '$';
    size_t inputIdx = 0;
    int step = 1;
    
    cout << "Step\tStack\t\t\tInput\t\t\tAction\n";
    cout << "====\t=====\t\t\t=====\t\t\t======\n";
    
    bool success = false;
    
    while (true) {
        int currentState = stateStack.back();
        char currentInput = input[inputIdx];
        
        cout << step++ << "\t";
        
        // Print symbol stack
        for (size_t i = 0; i < symbolStack.size(); i++) {
            cout << symbolStack[i];
        }
        cout << " (";
        for (size_t i = 0; i < stateStack.size(); i++) {
            cout << stateStack[i];
            if (i < stateStack.size() - 1) cout << " ";
        }
        cout << ")";
        
        // Padding for alignment
        int padding = 25 - (symbolStack.size() + stateStack.size() * 2);
        for (int i = 0; i < padding; i++) cout << " ";
        
        cout << input.substr(inputIdx);
        
        // Padding for input
        padding = 24 - input.substr(inputIdx).size();
        for (int i = 0; i < padding; i++) cout << " ";
        
        // Check if action exists
        if (actionTable.count(currentState) == 0 || 
            actionTable.at(currentState).count(currentInput) == 0) {
            cout << "ERROR: No action defined\n";
            cout << "\n*** Parsing Failed! ***\n\n";
            return;
        }
        
        const Action& action = actionTable.at(currentState).at(currentInput);
        
        if (action.type == "SHIFT") {
            cout << "SHIFT " << action.state << "\n";
            symbolStack.push_back(currentInput);
            stateStack.push_back(action.state);
            inputIdx++;
            
        } else if (action.type == "REDUCE") {
            cout << "REDUCE " << action.lhs << " -> " << action.rhs << "\n";
            
            // Pop symbols and states
            for (size_t i = 0; i < action.rhs.size(); i++) {
                symbolStack.pop_back();
                stateStack.pop_back();
            }
            
            symbolStack.push_back(action.lhs);
            
            // Get goto state
            int topState = stateStack.back();
            if (gotoTable.count(topState) > 0 && 
                gotoTable.at(topState).count(action.lhs) > 0) {
                stateStack.push_back(gotoTable.at(topState).at(action.lhs));
            } else {
                cout << "\n*** ERROR: No GOTO defined for state " << topState 
                     << " and symbol " << action.lhs << " ***\n\n";
                return;
            }
            
        } else if (action.type == "ACCEPT") {
            cout << "ACCEPT\n";
            success = true;
            break;
            
        } else {
            cout << "ERROR: Invalid action\n";
            cout << "\n*** Parsing Failed! ***\n\n";
            return;
        }
    }
    
    if (success) {
        cout << "\n";
        cout << "========================================\n";
        cout << "  String ACCEPTED! Parsing Successful  \n";
        cout << "========================================\n\n";
    }
}

int main() {
    // Define the grammar
    Grammar grammar;
    grammar.rules['S'] = {"CC"};
    grammar.rules['C'] = {"cC", "d"};
    grammar.rules['X'] = {"S"};  // Augmented production

    grammar.print();

    // Define terminals and non-terminals
    unordered_set<char> terminals = {'c', 'd', '$'};
    unordered_set<char> non_terminals = {'S', 'C', 'X'};

    // Compute FIRST sets
    map<char, unordered_set<char>> first = computeFirst(grammar, non_terminals);

    cout << "FIRST Sets:\n";
    cout << "===========\n";
    vector<char> sorted_nts = {'S', 'C'};
    for (char nt : sorted_nts) {
        cout << "FIRST(" << nt << ") = { ";
        vector<char> symbols(first[nt].begin(), first[nt].end());
        sort(symbols.begin(), symbols.end());
        for (size_t i = 0; i < symbols.size(); i++) {
            cout << symbols[i];
            if (i < symbols.size() - 1) cout << ", ";
        }
        cout << " }\n";
    }
    cout << endl;

    // Create initial state I0 with augmented start production
    State I0;
    LR1Item startItem;
    startItem.lhs = 'X';
    startItem.rhs = "S";
    startItem.dot = 0;
    startItem.lookahead = '$';  // Initial lookahead
    I0.items.insert(startItem);

    computeClosure(&I0, grammar, terminals, non_terminals, first);

    states.add(I0, stateCounter);
    stateCounter++;

    queue<State> stateQueue;
    stateQueue.push(I0);

    cout << "Generating LR(1) States with Lookaheads...\n";
    cout << "===========================================\n\n";

    // Generate all LR(1) states
    while (!stateQueue.empty()) {
        State currentState = stateQueue.front();
        stateQueue.pop();
        
        int currentStateNum = states.getStateNum(currentState);

        unordered_set<char> symbols = getSymbols(currentState);

        for (char symbol : symbols) {
            State newState = computeGoto(currentState, symbol, grammar, terminals, non_terminals, first);

            if (!newState.items.empty()) {
                if (!states.find(newState)) {
                    cout << "GOTO(I" << currentStateNum << ", " << symbol << ") = I" << stateCounter << " (NEW STATE)\n";
                    states.add(newState, stateCounter);
                    stateCounter++;
                    stateQueue.push(newState);
                } else {
                    int existingStateNum = states.getStateNum(newState);
                    cout << "GOTO(I" << currentStateNum << ", " << symbol << ") = I" << existingStateNum << " (EXISTING)\n";
                }
            }
        }
    }

    cout << endl;
    states.print();

    // Build CLR parsing table
    map<int, map<char, Action>> actionTable;
    map<int, map<char, int>> gotoTable;
    
    buildCLRParsingTable(actionTable, gotoTable, grammar, states, terminals, non_terminals, first);
    printCLRParsingTable(actionTable, gotoTable, terminals, non_terminals, stateCounter);

    // Parse the input string "ccdd"
    parseInput("ccdd", actionTable, gotoTable, grammar);

    cout << "===========================================\n";
    cout << "CLR Parser Implementation Complete!\n";
    cout << "Submission Date: March 4, 2026\n";
    cout << "===========================================\n";

    return 0;
}
