#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Structure for Three-Address Code
struct Instruction {
    string op;
    string arg1;
    string arg2;
    string result;
    string label;
};

// Global counter for labels and temporary variables
int labelCounter = 1;
int tempCounter = 1;

string newLabel() {
    return "L" + to_string(labelCounter++);
}

string newTemp() {
    return "t" + to_string(tempCounter++);
}

// Global list of instructions
vector<Instruction> tac;

void emit(string op, string arg1, string arg2, string result, string label = "") {
    tac.push_back({op, arg1, arg2, result, label});
}

void printTAC() {
    cout << "\n--- Three Address Code ---\n";
    for (int i = 0; i < tac.size(); i++) {
        if (!tac[i].label.empty()) {
            cout << tac[i].label << ": ";
        } else {
            cout << "    ";
        }

        if (tac[i].op == "=") {
            cout << tac[i].result << " = " << tac[i].arg1 << endl;
        } else if (tac[i].op == "if") {
            cout << "if " << tac[i].arg1 << " goto " << tac[i].result << endl;
        } else if (tac[i].op == "goto") {
            cout << "goto " << tac[i].result << endl;
        } else if (tac[i].op == "[]") {
            cout << tac[i].result << " = " << tac[i].arg1 << "[" << tac[i].arg2 << "]" << endl;
        } else if (tac[i].op == "[]=") {
            cout << tac[i].arg1 << "[" << tac[i].arg2 << "] = " << tac[i].result << endl;
        } else if (tac[i].op == "[][]") {
            cout << tac[i].result << " = " << tac[i].arg1 << "[" << tac[i].arg2 << "]" << endl;
        } else if (tac[i].op == "[][]=") {
            cout << tac[i].arg1 << "[" << tac[i].arg2 << "] = " << tac[i].result << endl;
        } else if (tac[i].op == "+" || tac[i].op == "-" || tac[i].op == "*" || tac[i].op == "/" || 
                   tac[i].op == "<" || tac[i].op == ">" || tac[i].op == "==" || tac[i].op == "<=" || tac[i].op == ">=") {
            cout << tac[i].result << " = " << tac[i].arg1 << " " << tac[i].op << " " << tac[i].arg2 << endl;
        } else {
            cout << tac[i].op << " " << tac[i].arg1 << " " << tac[i].arg2 << " " << tac[i].result << endl;
        }
    }
}

void printQuadruples() {
    cout << "\n--- Quadruples ---\n";
    cout << left << setw(5) << "Index" << setw(10) << "Op" << setw(10) << "Arg1" << setw(10) << "Arg2" << setw(10) << "Result" << endl;
    for (int i = 0; i < tac.size(); i++) {
        cout << left << setw(5) << i << setw(10) << tac[i].op << setw(10) << tac[i].arg1 << setw(10) << tac[i].arg2 << setw(10) << tac[i].result << endl;
    }
}

void printTriples() {
    cout << "\n--- Triples ---\n";
    cout << left << setw(5) << "Index" << setw(10) << "Op" << setw(10) << "Arg1" << setw(10) << "Arg2" << endl;
    for (int i = 0; i < tac.size(); i++) {
        string arg1 = tac[i].arg1;
        string arg2 = tac[i].arg2;
        
        // This is a simplified Triple representation showing the operation flow
        cout << left << setw(5) << i << setw(10) << tac[i].op << setw(10) << arg1 << setw(10) << arg2 << endl;
    }
}

void problemA() {
    cout << "\n--- Problem (a): Student Marks Loop ---\n";
    tac.clear();
    labelCounter = 1;
    tempCounter = 1;

    // Initialization
    emit("=", "0", "", "i");
    emit("=", "0", "", "sum");
    emit("=", "0", "", "fail_count");

    string startLabel = newLabel();
    string trueLabel = newLabel();
    string falseLabel = newLabel();
    string nextLabel = newLabel();
    string endLabel = newLabel();

    // While loop start
    string t1 = newTemp();
    emit("<", "i", "n", t1, startLabel);
    emit("if", t1 + " == 0", "", endLabel);

    // If marks[i] > 50
    string t3 = newTemp();
    emit("[]", "marks", "i", t3);
    
    string t4 = newTemp();
    emit(">", t3, "50", t4);
    emit("if", t4 + " == 0", "", falseLabel);

    // True part: sum = sum + marks[i]
    string t5 = newTemp();
    emit("+", "sum", t3, t5, trueLabel);
    emit("=", t5, "", "sum");
    emit("goto", "", "", nextLabel);

    // False part: fail_count = fail_count + 1
    string t6 = newTemp();
    emit("+", "fail_count", "1", t6, falseLabel);
    emit("=", t6, "", "fail_count");

    // Increment i and jump back
    emit("+", "i", "1", "i", nextLabel);
    emit("goto", "", "", startLabel);

    // Dummy instruction for end label
    emit("LABEL_END", "", "", "", endLabel);

    printTAC();
    printQuadruples();
    printTriples();
}

void problemB() {
    cout << "\n--- Problem (b): Scholarship Eligibility ---\n";
    tac.clear();
    labelCounter = 1;
    tempCounter = 1;

    string labelElseIf = newLabel();
    string labelPartial = newLabel();
    string labelElse = newLabel();
    string labelEnd = newLabel();

    // if (marks > 80 AND attendance > 90)
    string t1 = newTemp();
    emit(">", "marks", "80", t1);
    emit("if", t1 + " == 0", "", labelElseIf);
    
    string t2 = newTemp();
    emit(">", "attendance", "90", t2);
    emit("if", t2 + " == 0", "", labelElseIf);

    // scholarship = 2
    emit("=", "2", "", "scholarship");
    emit("goto", "", "", labelEnd);

    // else if (marks > 70 OR sports == 1)
    string t3 = newTemp();
    emit(">", "marks", "70", t3, labelElseIf);
    emit("if", t3 + " != 0", "", labelPartial);

    string t4 = newTemp();
    emit("==", "sports", "1", t4);
    emit("if", t4 + " == 0", "", labelElse);

    // scholarship = 1
    emit("=", "1", "", "scholarship", labelPartial);
    emit("goto", "", "", labelEnd);

    // else scholarship = 0
    emit("=", "0", "", "scholarship", labelElse);

    emit("LABEL_END", "", "", "", labelEnd);

    printTAC();
    printQuadruples();
    printTriples();
}

void problemC() {
    cout << "\n--- Problem (c): Array Search with Break ---\n";
    tac.clear();
    labelCounter = 1;
    tempCounter = 1;

    emit("=", "0", "", "i");
    emit("=", "0", "", "found");

    string startLabel = newLabel();
    string endLabel = newLabel();
    string matchLabel = newLabel();
    string nextLabel = newLabel();

    // while (i < n)
    string t1 = newTemp();
    emit("<", "i", "n", t1, startLabel);
    emit("if", t1 + " == 0", "", endLabel);

    // if (A[i] == key)
    string t3 = newTemp();
    emit("[]", "A", "i", t3);
    
    string t4 = newTemp();
    emit("==", t3, "key", t4);
    emit("if", t4 + " != 0", "", matchLabel);
    emit("goto", "", "", nextLabel);

    // found = 1; break;
    emit("=", "1", "", "found", matchLabel);
    emit("goto", "", "", endLabel);

    // i = i + 1
    emit("+", "i", "1", "i", nextLabel);
    emit("goto", "", "", startLabel);

    emit("LABEL_END", "", "", "", endLabel);

    printTAC();
}

void problemD() {
    cout << "\n--- Problem (d): Matrix Replacement ---\n";
    tac.clear();
    labelCounter = 1;
    tempCounter = 1;

    emit("=", "0", "", "i");

    string loopIStart = newLabel();
    string loopJStart = newLabel();
    string nextI = newLabel();
    string nextJ = newLabel();
    string endLabel = newLabel();

    // for(i = 0; i < m; i++)
    string ti = newTemp();
    emit("<", "i", "m", ti, loopIStart);
    emit("if", ti + " == 0", "", endLabel);

    emit("=", "0", "", "j");

    // for(j = 0; j < n; j++)
    string tj = newTemp();
    emit("<", "j", "n", tj, loopJStart);
    emit("if", tj + " == 0", "", nextI);

    // if (M[i][j] < 0)
    string t4 = newTemp();
    emit("[][]", "M", "i,j", t4);
    
    string t5 = newTemp();
    emit("<", t4, "0", t5);
    emit("if", t5 + " == 0", "", nextJ);

    // M[i][j] = 0
    emit("[][]=", "M", "i,j", "0");

    // next j
    emit("+", "j", "1", "j", nextJ);
    emit("goto", "", "", loopJStart);

    // next i
    emit("+", "i", "1", "i", nextI);
    emit("goto", "", "", loopIStart);

    emit("LABEL_END", "", "", "", endLabel);

    printTAC();
}

int main() {
    problemA();
    problemB();
    problemC();
    problemD();
    return 0;
}
