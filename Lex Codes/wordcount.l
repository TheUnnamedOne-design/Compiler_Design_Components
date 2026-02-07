%{
#include<stdio.h>
int words=0;
%}

%%
[a-zA-Z]+ {words++;}
[\t\n]+ {}
. {}
%%

int yywrap() {
    return 1; 
}


int main()
{
    yylex();
    printf("Number of words = %d\n",words);
    return 0;
}