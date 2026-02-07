%{
#include<stdio.h>
int operands=0;
int Operators=0;
%}


KEYWORD auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while


%%

{KEYWORD} {}

[\+\-\*\/\=] {
    printf("Operators : %c\n",yytext[0]);
    Operators++;
}
[a-zA-z0-9][a-zA-z0-9]* {
    printf("operands : %s\n",yytext);
    operands++;
}
. {}
\n {}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("operands count : %d\n",operands);
    printf("Operators count : %d\n",Operators);
    return 0;
}