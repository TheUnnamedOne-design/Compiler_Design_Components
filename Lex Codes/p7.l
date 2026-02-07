%{
#include<stdio.h>
int keywords=0;
int identifiers=0;
%}


KEYWORD auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while
ID [a-zA-Z][a-zA-z0-9_]*

%%

{KEYWORD} {
    keywords++;
    printf("Keyword %s\n",(yytext));
}
{ID} {
    identifiers++;
    printf("Identifier : %s\n", yytext);
}
. {}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    printf("keywords count : %d\n",keywords);
    printf("identifiers count : %d\n",identifiers);
    return 0;
}