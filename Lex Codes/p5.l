%{
#include<stdio.h>
#include <ctype.h> 
int lowers=0;
%}

%%
[a-z] {
    lowers++;
    printf("%c",toupper(yytext[0]));
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
    return 0;
}