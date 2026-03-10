#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <map>
using namespace std;

struct Quadruple {
    string op, arg1, arg2, result;
    
    Quadruple(string o, string a1, string a2, string r) 
        : op(o), arg1(a1), arg2(a2), result(r) {}
};

struct Triple {
    string op, arg1, arg2;
    
    Triple(string o, string a1, string a2) 
        : op(o), arg1(a1), arg2(a2) {}
};

struct Token {
    string value;
    bool isOperator;
    
    Token(string v, bool isOp) : value(v), isOperator(isOp) {}
};

class CodeGenerator {
private:
    vector<Quadruple> quads;
    vector<Triple> triples;
    vector<int> indirectPointers;
    int tempCounter;
    int labelCounter;
    int tripleIndex;

public:
    CodeGenerator() : tempCounter(1), labelCounter(1), tripleIndex(0) {}

    string newTemp() {
        return "t" + to_string(tempCounter++);
    }

    string newLabel() {
        return "L" + to_string(labelCounter++);
    }

    void emit(string op, string arg1, string arg2, string result) {
        quads.push_back(Quadruple(op, arg1, arg2, result));
    }

    void emitTriple(string op, string arg1, string arg2) {
        triples.push_back(Triple(op, arg1, arg2));
        indirectPointers.push_back(triples.size() - 1);
    }

    void reset() {
        quads.clear();
        triples.clear();
        indirectPointers.clear();
        tempCounter = 1;
        labelCounter = 1;
        tripleIndex = 0;
    }

    int getPrecedence(string op) {
        if (op == "^") return 3;
        if (op == "*" || op == "/" || op == "%") return 2;
        if (op == "+" || op == "-") return 1;
        return 0;
    }

    bool isRightAssociative(string op) {
        return op == "^";
    }

    vector<Token> tokenize(string expr) {
        vector<Token> tokens;
        string current = "";
        
        for (size_t i = 0; i < expr.length(); i++) {
            char ch = expr[i];
            
            if (isspace(ch)) continue;
            
            if (isalnum(ch)) {
                current += ch;
            } else {
                if (!current.empty()) {
                    tokens.push_back(Token(current, false));
                    current = "";
                }
                tokens.push_back(Token(string(1, ch), true));
            }
        }
        
        if (!current.empty()) {
            tokens.push_back(Token(current, false));
        }
        
        return tokens;
    }

    vector<string> infixToPostfix(string infix) {
        vector<Token> tokens = tokenize(infix);
        vector<string> postfix;
        stack<string> operatorStack;
        
        for (const auto& token : tokens) {
            if (!token.isOperator || isalnum(token.value[0])) {
                postfix.push_back(token.value);
            }
            else if (token.value == "(") {
                operatorStack.push(token.value);
            }
            else if (token.value == ")") {
                while (!operatorStack.empty() && operatorStack.top() != "(") {
                    postfix.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                if (!operatorStack.empty()) {
                    operatorStack.pop();
                }
            }
            else {
                while (!operatorStack.empty() && 
                       operatorStack.top() != "(" &&
                       (getPrecedence(operatorStack.top()) > getPrecedence(token.value) ||
                        (getPrecedence(operatorStack.top()) == getPrecedence(token.value) && 
                         !isRightAssociative(token.value)))) {
                    postfix.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.push(token.value);
            }
        }
        
        while (!operatorStack.empty()) {
            postfix.push_back(operatorStack.top());
            operatorStack.pop();
        }
        
        return postfix;
    }

    string evaluatePostfixAndGenerate3AC(vector<string> postfix, string resultVar) {
        stack<string> operandStack;
        map<int, string> tripleMap;
        
        for (const auto& token : postfix) {
            if (isalnum(token[0])) {
                operandStack.push(token);
            } else {
                string arg2 = operandStack.top(); operandStack.pop();
                string arg1 = operandStack.top(); operandStack.pop();
                
                string temp = newTemp();
                
                emit(token, arg1, arg2, temp);
                
                string tripleArg1 = arg1;
                string tripleArg2 = arg2;
                
                if (arg1[0] == 't' && arg1.length() > 1) {
                    int tempNum = stoi(arg1.substr(1)) - 1;
                    tripleArg1 = "(" + to_string(tempNum) + ")";
                }
                if (arg2[0] == 't' && arg2.length() > 1) {
                    int tempNum = stoi(arg2.substr(1)) - 1;
                    tripleArg2 = "(" + to_string(tempNum) + ")";
                }
                
                emitTriple(token, tripleArg1, tripleArg2);
                
                operandStack.push(temp);
            }
        }
        
        string finalTemp = operandStack.top();
        
        emit("=", finalTemp, "", resultVar);
        
        string tripleRef = finalTemp;
        if (finalTemp[0] == 't' && finalTemp.length() > 1) {
            int tempNum = stoi(finalTemp.substr(1)) - 1;
            tripleRef = "(" + to_string(tempNum) + ")";
        }
        emitTriple("=", tripleRef, resultVar);
        
        return finalTemp;
    }

    void generateFromInfix(string lhs, string rhs, string exprTitle, bool printDataStructures = false) {
        cout << "\n";
        printSeparator();
        cout << exprTitle << endl;
        printSeparator();
        
        reset();
        
        vector<string> postfix = infixToPostfix(rhs);
        evaluatePostfixAndGenerate3AC(postfix, lhs);
        
        printThreeAddressCode();
        
        if (printDataStructures) {
            printQuadruples();
            printTriples();
            printIndirectTriples();
        }
    }

    void printSeparator(char ch = '=', int width = 80) {
        cout << string(width, ch) << endl;
    }

    void printQuadruples() {
        cout << "\nQUADRUPLES:\n";
        cout << setw(8) << "Op" << " | " 
             << setw(10) << "Arg1" << " | " << setw(10) << "Arg2" << " | " 
             << setw(10) << "Result" << endl;
        printSeparator('-', 50);
        
        for (size_t i = 0; i < quads.size(); i++) {
            cout << setw(8) << quads[i].op << " | "
                 << setw(10) << quads[i].arg1 << " | "
                 << setw(10) << quads[i].arg2 << " | "
                 << setw(10) << quads[i].result << endl;
        }
    }

    void printTriples() {
        cout << "\nTRIPLES:\n";
        cout << setw(6) << "Index" << " | " << setw(8) << "Op" << " | " 
             << setw(12) << "Arg1" << " | " << setw(12) << "Arg2" << endl;
        printSeparator('-', 50);
        
        for (size_t i = 0; i < triples.size(); i++) {
            cout << "(" << i << ")" << setw(3) << " | " 
                 << setw(8) << triples[i].op << " | "
                 << setw(12) << triples[i].arg1 << " | "
                 << setw(12) << triples[i].arg2 << endl;
        }
    }

    void printIndirectTriples() {
        cout << "\nINDIRECT TRIPLES:\n";
        cout << setw(10) << "Statement" << " | " << setw(10) << "Pointer" << endl;
        printSeparator('-', 25);
        
        for (size_t i = 0; i < indirectPointers.size(); i++) {
            cout << setw(10) << i << " | " 
                 << setw(10) << "(" + to_string(indirectPointers[i]) + ")" << endl;
        }
    }

    void printThreeAddressCode() {
        cout << "\nTHREE ADDRESS CODE:\n";
        printSeparator('-', 40);
        
        for (const auto& q : quads) {
            if (q.op == "=") {
                cout << q.result << " = " << q.arg1 << endl;
            } else if (q.op == "if") {
                cout << "if " << q.arg1 << " " << q.arg2 << " " << q.result << endl;
            } else if (q.op == "goto") {
                cout << "goto " << q.result << endl;
            } else if (q.op == "label") {
                cout << q.result << ":" << endl;
            } else if (q.arg2.empty()) {
                cout << q.result << " = " << q.op << " " << q.arg1 << endl;
            } else {
                cout << q.result << " = " << q.arg1 << " " << q.op << " " << q.arg2 << endl;
            }
        }
    }
};

void generateExpression1(CodeGenerator& gen) {
    string lhs = "p";
    string rhs = "q*r+q-r+(t*u+v*w)";
    string title = "EXPRESSION 1: p = q * r + q - r + (t * u + v * w)";
    
    gen.generateFromInfix(lhs, rhs, title, true);
}

void generateExpression2(CodeGenerator& gen) {
    string lhs = "x";
    string rhs = "m+(n^o)/((p-q)/(r+(n^o))%s/t)";
    string title = "EXPRESSION 2: x = (m + (n^o) / ((p-q)/(r+(n^o)) % s/t))";
    
    gen.generateFromInfix(lhs, rhs, title, false);
}

void generateExpression3(CodeGenerator& gen) {
    cout << "\n\n";
    gen.printSeparator();
    cout << "EXPRESSION 3: if x < y, p = p + 1 else q = q + 1\n";
    gen.printSeparator();
    
    gen.reset();
    
    string t1 = gen.newTemp();
    gen.emit("<", "x", "y", t1);
    gen.emitTriple("<", "x", "y");
    
    string L1 = gen.newLabel();
    string L2 = gen.newLabel();
    
    gen.emit("if", t1, "goto", L1);
    gen.emitTriple("if", "(0)", L1);
    
    string t2 = gen.newTemp();
    gen.emit("+", "q", "1", t2);
    gen.emitTriple("+", "q", "1");
    
    gen.emit("=", t2, "", "q");
    gen.emitTriple("=", "(2)", "q");
    
    gen.emit("goto", "", "", L2);
    gen.emitTriple("goto", "", L2);
    
    gen.emit("label", "", "", L1);
    gen.emitTriple("label", "", L1);
    
    string t3 = gen.newTemp();
    gen.emit("+", "p", "1", t3);
    gen.emitTriple("+", "p", "1");
    
    gen.emit("=", t3, "", "p");
    gen.emitTriple("=", "(6)", "p");
    
    gen.emit("label", "", "", L2);
    gen.emitTriple("label", "", L2);
    
    gen.printThreeAddressCode();
}

int main() {
    CodeGenerator generator;
    
    generateExpression1(generator);
    generateExpression2(generator);
    generateExpression3(generator);

    
    return 0;
}
