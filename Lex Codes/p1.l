%{
#include<stdio.h>
int chars=0;
%}

%%
. {chars++;}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("Character count : %d\n",chars);
    return 0;
}