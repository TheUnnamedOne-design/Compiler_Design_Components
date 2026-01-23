#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> leading, trailing;

bool isTerminal(char c) {
    return !(c >= 'A' && c <= 'Z');
}

void findLeading(char nt) {
    for (auto p : grammar[nt]) {

        for (int i = 0; i < p.length(); i++) {
            if (isTerminal(p[i])) {
                leading[nt].insert(p[i]);
                break;
            } else {
                if (p[i] != nt) {
                    findLeading(p[i]);
                    leading[nt].insert(
                        leading[p[i]].begin(),
                        leading[p[i]].end()
                    );
                }
            }
        }
    }
}

void findTrailing(char nt) {
    for (auto p : grammar[nt]) {

        for (int i = p.length() - 1; i >= 0; i--) {
            if (isTerminal(p[i])) {
                trailing[nt].insert(p[i]);
                break;
            } else {
                if (p[i] != nt) {
                    findTrailing(p[i]);
                    trailing[nt].insert(
                        trailing[p[i]].begin(),
                        trailing[p[i]].end()
                    );
                }
            }
        }
    }
}

int main() {

    grammar['E'] = {
        "E+E",
        "E-E",
        "E*E",
        "E/E",
        "(E)",
        "i"
    };

    findLeading('E');
    findTrailing('E');

    cout << "LEADING(E) = { ";
    for (char c : leading['E']) cout << c << " ";
    cout << "}\n";

    cout << "TRAILING(E) = { ";
    for (char c : trailing['E']) cout << c << " ";
    cout << "}\n";

    return 0;
}
