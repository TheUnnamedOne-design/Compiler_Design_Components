%{
#include<stdio.h>
int digits=0;
%}

%%
[1-9] {digits++;}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("digits count : %d\n",digits);
    return 0;
}