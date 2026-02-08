%{
#include<stdio.h>
int keyword_count = 0;
int identifier_count = 0;
int operator_count = 0;
int constant_count = 0;
int special_count = 0;
%}

%%
"//".* {}
"/*"([^*]|\*+[^*/])*"*/" {}

"int"|"float"|"char"|"double"|"void"|"if"|"else"|"while"|"for"|"do"|"return"|"break"|"continue"|"switch"|"case"|"default"|"struct"|"const"|"sizeof" {
    printf("KEYWORD: %s\n", yytext);
    keyword_count++;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("IDENTIFIER: %s\n", yytext);
    identifier_count++;
}

[0-9]+\.[0-9]+ {
    printf("FLOAT CONSTANT: %s\n", yytext);
    constant_count++;
}

[0-9]+ {
    printf("INTEGER CONSTANT: %s\n", yytext);
    constant_count++;
}

"+"|"-"|"*"|"/"|"%"|"="|"=="|"!="|"<"|">"|"<="|">="|"&&"|"||"|"!"|"++"|"--" {
    printf("OPERATOR: %s\n", yytext);
    operator_count++;
}

"{"|"}"|"("|")"|"["|"]"|";"|","|"."|"#" {
    printf("SPECIAL SYMBOL: %s\n", yytext);
    special_count++;
}

[ \t\n] {}
. {}

%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter C program (Press Ctrl+D to end):\n");
    yylex();
    printf("\n=== LEXICAL ANALYSIS SUMMARY ===\n");
    printf("Keywords: %d\n", keyword_count);
    printf("Identifiers: %d\n", identifier_count);
    printf("Constants: %d\n", constant_count);
    printf("Operators: %d\n", operator_count);
    printf("Special Symbols: %d\n", special_count);
    return 0;
}
