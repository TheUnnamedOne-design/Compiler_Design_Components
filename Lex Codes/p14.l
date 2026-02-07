%{
#include <stdio.h>
%}

/* Definitions */
VALID_ID   [a-zA-Z_][a-zA-Z0-9_]*
INVALID_ID [0-9][a-zA-Z0-9_]*

%%

{VALID_ID}      { printf("Valid Identifier: %s\n", yytext); }
{INVALID_ID}    { printf("Invalid Identifier (starts with digit): %s\n", yytext); }
[ \t\n]         {}
.               { printf("Invalid Character/Identifier: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

int main() {
    printf("Enter identifiers to validate (Ctrl+D to exit):\n");
    yylex();
    return 0;
}