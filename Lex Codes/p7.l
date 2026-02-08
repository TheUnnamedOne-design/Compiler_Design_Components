%{
#include<stdio.h>
int keywords=0;
int identifiers=0;
int strings=0;
%}


KEYWORD auto|break|case|char|const|continue|printf|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while
ID      [a-zA-Z_][a-zA-Z0-9_]*

STRING  \"[^\"]*\"

%%
\n      {}
[ \t]+  {}

"//".* {}
"/*"([^*]|\*+[^*/])*"*/" {}

{STRING} {
    strings++;
    printf("String Literal: %s\n", yytext);
}

{KEYWORD} {
    keywords++;
    printf("Keyword: %s\n", yytext);
}

{ID} {
    identifiers++;
    printf("Identifier: %s\n", yytext);
}

. {}
%%

int yywrap() { return 1; }

int main() {
    yylex();
    printf("\n--- Final Counts ---\n");
    printf("Keywords: %d\n", keywords);
    printf("Identifiers: %d\n", identifiers);
    printf("Strings: %d\n", strings);
    return 0;
}