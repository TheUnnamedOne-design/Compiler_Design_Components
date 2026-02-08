%{
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_SYMBOLS 100

char symbol_table[MAX_SYMBOLS][50];
int symbol_count = 0;

int search_symbol(char *str) {
    for(int i = 0; i < symbol_count; i++) {
        if(strcmp(symbol_table[i], str) == 0) {
            return 1; // Found
        }
    }
    return 0; // Not found
}

void insert_symbol(char *str) {
    if(!search_symbol(str)) {
        strcpy(symbol_table[symbol_count], str);
        symbol_count++;
        printf("Inserted '%s' into symbol table\n", str);
    }
}

void display_symbol_table() {
    printf("\n========== SYMBOL TABLE ==========\n");
    printf("Index\tSymbol\n");
    printf("==================================\n");
    for(int i = 0; i < symbol_count; i++) {
        printf("%d\t%s\n", i+1, symbol_table[i]);
    }
    printf("==================================\n");
    printf("Total symbols: %d\n", symbol_count);
}
%}

%%
"//".* {}
"/*"([^*]|\*+[^*/])*"*/" {}

"int"|"float"|"char"|"double"|"void"|"if"|"else"|"while"|"for"|"do"|"return"|"break"|"continue"|"switch"|"case"|"default"|"struct"|"long"|"short"|"unsigned"|"signed"|"const"|"sizeof"|"static"|"auto"|"register"|"extern" {
    printf("Keyword: %s (not added to symbol table)\n", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("Identifier found: %s -> ", yytext);
    insert_symbol(yytext);
}

[0-9]+ {}
[0-9]+\.[0-9]+ {}
"+"|"-"|"*"|"/"|"%"|"="|"=="|"!="|"<"|">"|"<="|">="|"&&"|"||"|"!"|"++"|"--" {}
"{"|"}"|"("|")"|"["|"]"|";"|","|"."|"#" {}
[ \t\n] {}
. {}

%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter code (Press Ctrl+D to end):\n");
    printf("==================================\n");
    yylex();
    display_symbol_table();
    return 0;
}
