%{
#include<stdio.h>
%}

%%
"//".* {}
"/*"([^*]|\*+[^*/])*"*/" {}

"int"|"float"|"char"|"double"|"void"|"if"|"else"|"while"|"for"|"do"|"return"|"break"|"continue"|"switch"|"case"|"default"|"struct"|"long"|"short"|"unsigned"|"signed"|"const"|"sizeof"|"static"|"auto"|"register"|"extern" {
    printf("<KEYWORD, %s>\n", yytext);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    printf("<IDENTIFIER, %s>\n", yytext);
}

[0-9]+\.[0-9]+([eE][+-]?[0-9]+)? {
    printf("<FLOAT_CONSTANT, %s>\n", yytext);
}

[0-9]+ {
    printf("<INTEGER_CONSTANT, %s>\n", yytext);
}

'[^']' {
    printf("<CHARACTER_CONSTANT, %s>\n", yytext);
}

\"([^\"\\]|\\.)*\" {
    printf("<STRING_LITERAL, %s>\n", yytext);
}

"++" {printf("<OPERATOR, ++>\n");}
"--" {printf("<OPERATOR, -->\n");}
"+" {printf("<OPERATOR, +>\n");}
"-" {printf("<OPERATOR, ->\n");}
"*" {printf("<OPERATOR, *>\n");}
"/" {printf("<OPERATOR, />\n");}
"%" {printf("<OPERATOR, %%>\n");}
"==" {printf("<OPERATOR, ==>\n");}
"!=" {printf("<OPERATOR, !=>\n");}
"<=" {printf("<OPERATOR, <=>\n");}
">=" {printf("<OPERATOR, >=>\n");}
"<" {printf("<OPERATOR, <>\n");}
">" {printf("<OPERATOR, >>\n");}
"&&" {printf("<OPERATOR, &&>\n");}
"||" {printf("<OPERATOR, ||>\n");}
"!" {printf("<OPERATOR, !>\n");}
"=" {printf("<OPERATOR, =>\n");}

"{" {printf("<DELIMITER, {>\n");}
"}" {printf("<DELIMITER, }>\n");}
"(" {printf("<DELIMITER, (>\n");}
")" {printf("<DELIMITER, )>\n");}
"[" {printf("<DELIMITER, [>\n");}
"]" {printf("<DELIMITER, ]>\n");}
";" {printf("<DELIMITER, ;>\n");}
"," {printf("<DELIMITER, ,>\n");}
"." {printf("<DELIMITER, .>\n");}
"#" {printf("<PREPROCESSOR, #>\n");}

[ \t\n] {}
. {printf("<UNKNOWN, %s>\n", yytext);}

%%

int yywrap()
{
    return 1;
}

int main()
{
    printf("Enter code (Press Ctrl+D to end):\n");
    printf("==========================================\n");
    yylex();
    printf("==========================================\n");
    printf("Token generation complete.\n");
    return 0;
}
