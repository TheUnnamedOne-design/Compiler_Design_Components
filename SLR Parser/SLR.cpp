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

        unordered_set<char> symbols = getSymbols(currentState);

        for (char symbol : symbols) {
            State newState = computeGoto(currentState, symbol, grammar, non_terminals);

            if (!newState.items.empty() && !states.find(newState)) {
                states.add(newState, stateCounter);
                stateCounter++;
                stateQueue.push(newState);
            }
        }
    }   

    cout << "LR(0) States:\n";
    cout << "=============\n\n";
    states.print();

    return 0;
}

