%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token ID NUM
%left '+' '-'
%left '*' '/'

%%
E : E '+' T
  | E '-' T
  | T
  ;

T : T '*' F
  | T '/' F
  | F
  ;

F : '(' E ')'
  | ID
  | NUM
  ;

%%

void yyerror(const char *s) {
    printf("Invalid expression!\n");
}

int main() {
    printf("Enter an arithmetic expression: ");
    if (yyparse() == 0)
        printf("Valid expression!\n");
    return 0;
}
