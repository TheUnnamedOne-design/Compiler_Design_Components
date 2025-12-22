#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <stack>
using namespace std;

struct State {
    int id;
    map<char, vector<State*>> trans;
    vector<State*> epsilon;
};

struct NFA {
    State* start;
    State* accept;
};

int state_id = 0;

State* newState() {
    State* s = new State();
    s->id = state_id++;
    return s;
}

NFA symbolNFA(char c) {
    State* s = newState();
    State* f = newState();
    s->trans[c].push_back(f);
    return {s, f};
}

NFA concatNFA(NFA a, NFA b) {
    a.accept->epsilon.push_back(b.start);
    return {a.start, b.accept};
}

NFA unionNFA(NFA a, NFA b) {
    State* s = newState();
    State* f = newState();
    s->epsilon.push_back(a.start);
    s->epsilon.push_back(b.start);
    a.accept->epsilon.push_back(f);
    b.accept->epsilon.push_back(f);
    return {s, f};
}

NFA starNFA(NFA a) {
    State* s = newState();
    State* f = newState();
    s->epsilon.push_back(a.start);
    s->epsilon.push_back(f);
    a.accept->epsilon.push_back(a.start);
    a.accept->epsilon.push_back(f);
    return {s, f};
}

set<State*> epsilonClosure(set<State*> states) {
    stack<State*> st;
    for(auto s : states) st.push(s);

    while(!st.empty()) {
        State* cur = st.top(); st.pop();
        for(State* e : cur->epsilon) {
            if(states.insert(e).second)
                st.push(e);
        }
    }
    return states;
}

bool simulateNFA(NFA nfa, string input) {
    set<State*> curr;
    curr.insert(nfa.start);
    curr = epsilonClosure(curr);

    for(char c : input) {
        set<State*> next;
        for(State* s : curr) {
            if(s->trans.count(c)) {
                for(State* t : s->trans[c])
                    next.insert(t);
            }
        }
        curr = epsilonClosure(next);
    }

    return curr.count(nfa.accept);
}

int main() {
    // Regular Expression: (a|b)*abb

    NFA a = symbolNFA('a');
    NFA b = symbolNFA('b');

    NFA a_or_b = unionNFA(symbolNFA('a'), symbolNFA('b'));
    NFA a_or_b_star = starNFA(a_or_b);

    NFA ab1 = concatNFA(symbolNFA('a'), symbolNFA('b'));
    NFA abb = concatNFA(ab1, symbolNFA('b'));

    NFA finalNFA = concatNFA(a_or_b_star, abb);

    string input;
    cout << "Enter input string: ";
    cin >> input;

    if(simulateNFA(finalNFA, input))
        cout << "String ACCEPTED\n";
    else
        cout << "String REJECTED\n";

    return 0;
}
