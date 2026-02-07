%{
/* Definitions section */
#include <stdio.h>
%}

%%
.   { printf("%s", yytext); }
\n  { printf("%s", yytext); }
%%

/* User Code section - replaces the need for -lfl */
int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}
