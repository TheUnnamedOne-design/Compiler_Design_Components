%{
#include<stdio.h>
#include<stdlib.h>
int valid = 1;
%}

%%
[ \t] {}

[0-9]+\.[0-9]+ {
    printf("Valid: Float constant %s\n", yytext);
}

[0-9]+ {
    printf("Valid: Integer constant %s\n", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("Valid: Identifier %s\n", yytext);
}

"+" {printf("Valid: Addition operator\n");}
"-" {printf("Valid: Subtraction operator\n");}
"*" {printf("Valid: Multiplication operator\n");}
"/" {printf("Valid: Division operator\n");}
"%" {printf("Valid: Modulus operator\n");}
"(" {printf("Valid: Left parenthesis\n");}
")" {printf("Valid: Right parenthesis\n");}

\n {
    if(valid) {
        printf("\n✓ Arithmetic expression is VALID\n");
    } else {
        printf("\n✗ Arithmetic expression is INVALID\n");
    }
    valid = 1;
}

. {
    printf("Invalid character: %s\n", yytext);
    valid = 0;
}

%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter arithmetic expression:\n");
    printf("(Press Ctrl+D to exit)\n");
    printf("=====================================\n");
    yylex();
    return 0;
}
