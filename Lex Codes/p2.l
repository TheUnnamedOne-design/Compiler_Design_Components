%{
#include<stdio.h>
int words=0;
%}

%%
[a-zA-Z]+ {words++;}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("Word count : %d\n",words);
    return 0;
}