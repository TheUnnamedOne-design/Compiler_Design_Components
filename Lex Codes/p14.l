%{
#include<stdio.h>
int email_count = 0;
%}

%%
[a-zA-Z0-9._+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,} {
    printf("Email ID found: %s\n", yytext);
    email_count++;
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
    printf("Enter text (Press Ctrl+D to end):\n");
    yylex();
    printf("\nTotal email IDs found: %d\n", email_count);
    return 0;
}
