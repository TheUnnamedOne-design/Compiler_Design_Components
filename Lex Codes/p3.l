%{
#include<stdio.h>
int lines=0;
%}

%%
\n {lines++;}
.  {}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("line count : %d\n",lines);
    return 0;
}