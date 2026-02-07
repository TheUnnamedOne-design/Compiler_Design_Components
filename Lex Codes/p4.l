%{
#include<stdio.h>
int vowels=0;
%}

%%
[aeiouAEIOU] {vowels++;}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("vowels count : %d\n",vowels);
    return 0;
}