%{
#include<stdio.h>
#include <ctype.h> 
int lowers=0;
int uppers=0;
%}

%%
[a-z] {
    lowers++;
}

[A-Z] {
    uppers++;
}

. {
    printf("%s", yytext);
}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("lowers count : %d\n",lowers);
    printf("uppers count : %d\n",uppers);
    return 0;
}