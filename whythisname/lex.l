%{
#include <stdio.h>
%}
%%
"auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|
"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|
"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|
"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|
"volatile"|"while"      { printf("Keyword : %s\n", yytext); }
[0-9]+                  { printf("Number : %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("Identifier : %s\n", yytext); }
[ \t\n]+                { /* Ignore white spaces */ }
";"|","|"("|")"|"{"|"}"|"["|"]"   
                        { printf("Punctuation : %s\n", yytext); }
"#"                     { printf("Preprocessor Directive : %s\n", yytext); }
"+"|"-"|"*"|"/"|"="|"<"|">"|"<<"|">>"  
                        { printf("Operator : %s\n", yytext); }
.                       { printf("Unknown : %s\n", yytext); }
%%
int yywrap() { return 1; }
int main() {
    yylex();
    return 0;
}
