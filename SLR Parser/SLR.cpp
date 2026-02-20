#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;



struct Grammar {
    map<char, vector<string>> rules;

    void print() const {
        for (auto &p : rules) {
            for (auto &rhs : p.second) {
                cout << p.first << " -> " << rhs << endl;
            }
        }
    }
};



struct Item {
    char lhs;
    string rhs;
    int dot;

    bool operator==(const Item& other) const {
        return lhs == other.lhs &&
               rhs == other.rhs &&
               dot == other.dot;
    }

    void print() const {
        cout << lhs << " -> ";
        for (int i = 0; i < rhs.size(); i++) {
            if (i == dot)
                cout << ".";
            cout << rhs[i];
        }
        if (dot == rhs.size())
            cout << ".";
        cout << endl;
    }
};



struct ItemHash {
    size_t operator()(const Item& item) const {
        size_t h = hash<char>()(item.lhs);

        for (char c : item.rhs)
            h ^= hash<char>()(c) + 0x9e3779b9 + (h << 6) + (h >> 2);

        h ^= hash<int>()(item.dot);

        return h;
    }
};


struct State {
    unordered_set<Item, ItemHash> items;

    bool operator==(const State& other) const {
        return items == other.items;
    }

    void print() const {
        for (const auto& item : items) {
            item.print();
        }
        cout << endl;
    }
};



struct StateHash {
    size_t operator()(const State& state) const {
        size_t h = 0;
        ItemHash item_hash;
        for (const auto& item : state.items) {
            h ^= item_hash(item);
        }
        return h;
    }
};



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
        sort(sorted_states.begin(), sorted_states.end(), 
             [](const pair<int, State>& a, const pair<int, State>& b) {
                 return a.first < b.first;
             });

        for (auto& p : sorted_states) {
            cout << "I" << p.first << ":" << endl;
            p.second.print();
        }
    }
};

StatesManager states;
int stateCounter = 0;



void computeClosure(State* state, const Grammar& grammar, const unordered_set<char>& non_terminals) {
    bool changed = true;
    while (changed) {
        changed = false;
        unordered_set<Item, ItemHash> newItems;

        for (const auto& item : state->items) {
            if (item.dot < item.rhs.size()) {
                char nextSymbol = item.rhs[item.dot];
                
                if (non_terminals.find(nextSymbol) != non_terminals.end()) {
                    if (grammar.rules.find(nextSymbol) != grammar.rules.end()) {
                        for (const auto& production : grammar.rules.at(nextSymbol)) {
                            Item newItem;
                            newItem.lhs = nextSymbol;
                            newItem.rhs = production;
                            newItem.dot = 0;
                            
                            if (state->items.find(newItem) == state->items.end()) {
                                newItems.insert(newItem);
                                changed = true;
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

State computeGoto(const State& state, char symbol, const Grammar& grammar, const unordered_set<char>& non_terminals) {
    State newState;

    for (const auto& item : state.items) {
        if (item.dot < item.rhs.size() && item.rhs[item.dot] == symbol) {
            Item newItem;
            newItem.lhs = item.lhs;
            newItem.rhs = item.rhs;
            newItem.dot = item.dot + 1;
            
            newState.items.insert(newItem);
        }
    }

    if (!newState.items.empty()) {
        computeClosure(&newState, grammar, non_terminals);
    }

    return newState;
}

unordered_set<char> getSymbols(const State& state) {
    unordered_set<char> symbols;
    for (const auto& item : state.items) {
        if (item.dot < item.rhs.size()) {
            symbols.insert(item.rhs[item.dot]);
        }
    }
    return symbols;
}

map<char, unordered_set<char>> computeFirst(const Grammar& grammar, const unordered_set<char>& non_terminals) {
    map<char, unordered_set<char>> first;
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& rule : grammar.rules) {
            char lhs = rule.first;
            
            for (const auto& rhs : rule.second) {
                if (rhs.empty()) {
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

map<char, unordered_set<char>> computeFollow(const Grammar& grammar, const unordered_set<char>& non_terminals, 
                                              const map<char, unordered_set<char>>& first, char startSymbol) {
    map<char, unordered_set<char>> follow;
    
    follow[startSymbol].insert('$');
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& rule : grammar.rules) {
            char lhs = rule.first;
            
            for (const auto& rhs : rule.second) {
                for (size_t i = 0; i < rhs.size(); i++) {
                    char symbol = rhs[i];
                    
                    if (non_terminals.find(symbol) == non_terminals.end()) {
                        continue;
                    }
                    
                    if (i + 1 < rhs.size()) {
                        char nextSymbol = rhs[i + 1];
                        
                        if (non_terminals.find(nextSymbol) == non_terminals.end()) {
                            if (follow[symbol].insert(nextSymbol).second) {
                                changed = true;
                            }
                        } else {
                            size_t oldSize = follow[symbol].size();
                            for (char c : first.at(nextSymbol)) {
                                if (c != 'e') {
                                    follow[symbol].insert(c);
                                }
                            }
                            if (follow[symbol].size() > oldSize) {
                                changed = true;
                            }
                            
                            if (first.at(nextSymbol).count('e') > 0) {
                                oldSize = follow[symbol].size();
                                for (char c : follow[lhs]) {
                                    follow[symbol].insert(c);
                                }
                                if (follow[symbol].size() > oldSize) {
                                    changed = true;
                                }
                            }
                        }
                    } else {
                        size_t oldSize = follow[symbol].size();
                        for (char c : follow[lhs]) {
                            follow[symbol].insert(c);
                        }
                        if (follow[symbol].size() > oldSize) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    
    return follow;
}

void printFollow(const map<char, unordered_set<char>>& follow) {
    cout << "FOLLOW Sets:\n";
    cout << "============\n";
    
    vector<char> sorted_keys;
    for (const auto& p : follow) {
        sorted_keys.push_back(p.first);
    }
    sort(sorted_keys.begin(), sorted_keys.end());
    
    for (char key : sorted_keys) {
        cout << "FOLLOW(" << key << ") = { ";
        vector<char> symbols(follow.at(key).begin(), follow.at(key).end());
        sort(symbols.begin(), symbols.end());
        for (size_t i = 0; i < symbols.size(); i++) {
            cout << symbols[i];
            if (i < symbols.size() - 1) cout << ", ";
        }
        cout << " }\n";
    }
    cout << endl;
}

struct Action {
    string type;
    int state;
    char lhs;
    string rhs;
    
    Action() : type("ERROR"), state(-1), lhs('\0'), rhs("") {}
};

void buildParsingTable(map<int, map<char, Action>>& actionTable,
                       map<int, map<char, int>>& gotoTable,
                       const Grammar& grammar,
                       const StatesManager& states,
                       const map<char, unordered_set<char>>& follow,
                       const unordered_set<char>& terminals,
                       const unordered_set<char>& non_terminals) {
    
    for (const auto& statePair : states.stateMap) {
        const State& state = statePair.first;
        int stateNum = statePair.second;
        
        for (const auto& item : state.items) {
            if (item.dot >= item.rhs.size()) {
                if (item.lhs == 'X' && item.rhs == "S") {
                    actionTable[stateNum]['$'].type = "ACCEPT";
                } else {
                    if (follow.count(item.lhs) > 0) {
                        for (char followSymbol : follow.at(item.lhs)) {
                            actionTable[stateNum][followSymbol].type = "REDUCE";
                            actionTable[stateNum][followSymbol].lhs = item.lhs;
                            actionTable[stateNum][followSymbol].rhs = item.rhs;
                        }
                    }
                }
            }
        }
        
        for (const auto& item : state.items) {
            if (item.dot < item.rhs.size()) {
                char nextSymbol = item.rhs[item.dot];
                
                State nextState = computeGoto(state, nextSymbol, grammar, non_terminals);
                if (!nextState.items.empty() && states.stateMap.count(nextState) > 0) {
                    int nextStateNum = states.stateMap.at(nextState);
                    
                    if (terminals.find(nextSymbol) != terminals.end()) {
                        actionTable[stateNum][nextSymbol].type = "SHIFT";
                        actionTable[stateNum][nextSymbol].state = nextStateNum;
                    } else if (non_terminals.find(nextSymbol) != non_terminals.end()) {
                        gotoTable[stateNum][nextSymbol] = nextStateNum;
                    }
                }
            }
        }
    }
}

void printParsingTable(const map<int, map<char, Action>>& actionTable,
                       const map<int, map<char, int>>& gotoTable,
                       const unordered_set<char>& terminals,
                       const unordered_set<char>& non_terminals,
                       int numStates) {
    
    cout << "SLR Parsing Table:\n";
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
    
    cout << "State\t";
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
    
    for (int i = 0; i < numStates; i++) {
        cout << i << "\t";
        
        for (char t : terminalsList) {
            if (actionTable.count(i) > 0 && actionTable.at(i).count(t) > 0) {
                const Action& action = actionTable.at(i).at(t);
                if (action.type == "SHIFT") {
                    cout << "s" << action.state << "\t";
                } else if (action.type == "REDUCE") {
                    cout << "r(" << action.lhs << "->" << action.rhs << ")\t";
                } else if (action.type == "ACCEPT") {
                    cout << "acc\t";
                }
            } else {
                cout << "\t";
            }
        }
        
        cout << "|\t";
        
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

void parseInput(string input, 
                const map<int, map<char, Action>>& actionTable,
                const map<int, map<char, int>>& gotoTable,
                const Grammar& grammar) {
    
    cout << "Parsing Input: " << input << endl;
    cout << "==========================================\n\n";
    
    vector<int> stateStack;
    vector<char> symbolStack;
    
    stateStack.push_back(0);
    symbolStack.push_back('#');
    
    input += '$';
    size_t inputIdx = 0;
    int step = 1;
    
    cout << "Step\tStack\t\t\tInput\t\t\tAction\n";
    cout << "----\t-----\t\t\t-----\t\t\t------\n";
    
    while (true) {
        int currentState = stateStack.back();
        char currentInput = input[inputIdx];
        
        cout << step++ << "\t";
        
        for (size_t i = 0; i < symbolStack.size(); i++) {
            cout << symbolStack[i];
        }
        cout << " (";
        for (size_t i = 0; i < stateStack.size(); i++) {
            cout << stateStack[i];
            if (i < stateStack.size() - 1) cout << " ";
        }
        cout << ")\t\t";
        
        cout << input.substr(inputIdx) << "\t\t\t";
        
        if (actionTable.count(currentState) == 0 || 
            actionTable.at(currentState).count(currentInput) == 0) {
            cout << "ERROR: No action defined\n";
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
            
            for (size_t i = 0; i < action.rhs.size(); i++) {
                symbolStack.pop_back();
                stateStack.pop_back();
            }
            
            symbolStack.push_back(action.lhs);
            
            int topState = stateStack.back();
            if (gotoTable.count(topState) > 0 && 
                gotoTable.at(topState).count(action.lhs) > 0) {
                stateStack.push_back(gotoTable.at(topState).at(action.lhs));
            } else {
                cout << "\nERROR: No GOTO defined for state " << topState 
                     << " and symbol " << action.lhs << "\n";
                return;
            }
            
        } else if (action.type == "ACCEPT") {
            cout << "ACCEPT\n";
            cout << "\nInput string parsed successfully!\n\n";
            return;
            
        } else {
            cout << "ERROR: Invalid action\n";
            return;
        }
    }
}

int main() {
    Grammar grammar;
    grammar.rules['S'] = {"L=R", "R"};
    grammar.rules['L'] = {"*R", "i"};
    grammar.rules['R'] = {"L"};
    grammar.rules['X'] = {"S"};

    cout << "Grammar:\n";
    grammar.print();
    cout << endl;

    unordered_set<char> terminals = {'=', '*', 'i', '$'};
    unordered_set<char> non_terminals = {'S', 'L', 'R', 'X'};

    map<char, unordered_set<char>> first = computeFirst(grammar, non_terminals);
    map<char, unordered_set<char>> follow = computeFollow(grammar, non_terminals, first, 'X');

    State I0;
    Item startItem;
    startItem.lhs = 'X';
    startItem.rhs = "S";
    startItem.dot = 0;
    I0.items.insert(startItem);

    computeClosure(&I0, grammar, non_terminals);

    states.add(I0, stateCounter);
    stateCounter++;

    queue<State> stateQueue;
    stateQueue.push(I0);

    cout << "Generating LR(0) States...\n\n";

    while (!stateQueue.empty()) {
        State currentState = stateQueue.front();
        stateQueue.pop();
        
        int currentStateNum = states.getStateNum(currentState);

        unordered_set<char> symbols = getSymbols(currentState);

        for (char symbol : symbols) {
            State newState = computeGoto(currentState, symbol, grammar, non_terminals);

            if (!newState.items.empty()) {
                if (!states.find(newState)) {
                    cout << "GOTO(I" << currentStateNum << ", " << symbol << ") = I" << stateCounter << " (NEW STATE)\n";
                    states.add(newState, stateCounter);
                    stateCounter++;
                    stateQueue.push(newState);
                } else {
                    int existingStateNum = states.getStateNum(newState);
                    cout << "GOTO(I" << currentStateNum << ", " << symbol << ") = I" << existingStateNum << " (EXISTING STATE)\n";
                }
            }
        }
    }   

    cout << "\nLR(0) States:\n";
    cout << "=============\n\n";
    states.print();

    printFollow(follow);

    map<int, map<char, Action>> actionTable;
    map<int, map<char, int>> gotoTable;
    
    buildParsingTable(actionTable, gotoTable, grammar, states, follow, terminals, non_terminals);
    printParsingTable(actionTable, gotoTable, terminals, non_terminals, stateCounter);

    parseInput("*i=i", actionTable, gotoTable, grammar);

    return 0;
}

