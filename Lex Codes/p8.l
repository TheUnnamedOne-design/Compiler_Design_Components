%{
#include<stdio.h>
int integers=0;
int floats=0;
%}

%%
[0-9]+\.[0-9]+ {
    printf("Float : %s\n",yytext);
    floats++;
}
[0-9]* {
    printf("Ineger : %s\n",yytext);
    integers++;
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
    printf("integers count : %d\n",integers);
    printf("floats count : %d\n",floats);
    return 0;
}