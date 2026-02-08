%{
#include<stdio.h>
int integers=0;
int floats=0;
%}

%%
"//".* {}
"/*"([^*]|\*+[^*/])*"*/" {}
"&&" {printf("Logical AND : %s\n",yytext);}
"||" {printf("Logical OR : %s\n",yytext);}
"!" {printf("Logical NOT : %s\n",yytext);}
"==" {printf("Relational Equal to : %s\n",yytext);}
"!=" {printf("Relational Not equal to : %s\n",yytext);}
"<=" {printf("Relational Less than or equal to : %s\n",yytext);}
">=" {printf("Relational Greater than or equal to : %s\n",yytext);}
"<" {printf("Relational Less than : %s\n",yytext);}
">" {printf("Relational Greater than : %s\n",yytext);}
. {}
.|\n {}
%%

int yywrap()
{
    return 1;
}

int main()
{
    yylex();
    return 0;
}