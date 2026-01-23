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

    cout << "Production 1\n";

    grammar.clear();
    leading.clear();
    trailing.clear();

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

    cout << "LEADING(E) : ";
    for (char c : leading['E']) cout << c << " ";
    cout << "\n";

    cout << "TRAILING(E) : ";
    for (char c : trailing['E']) cout << c << " ";
    cout << "\n\n";


    cout << "Production 2\n";

    grammar.clear();
    leading.clear();
    trailing.clear();

    grammar['S'] = {
        "(L)",
        "a"
    };

    grammar['L'] = {
        "L,S",
        "S"
    };

    findLeading('S');
    findLeading('L');
    findTrailing('S');
    findTrailing('L');

    cout << "LEADING(S) : ";
    for (char c : leading['S']) cout << c << " ";
    cout << "\n";

    cout << "TRAILING(S) : ";
    for (char c : trailing['S']) cout << c << " ";
    cout << "\n";

    cout << "LEADING(L) : ";
    for (char c : leading['L']) cout << c << " ";
    cout << "\n";

    cout << "TRAILING(L) : ";
    for (char c : trailing['L']) cout << c << " ";
    cout << "\n";

    return 0;
}
