%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int yylex(void);
void yyerror(const char *s);
bool error_flag = false;  // track if an error occurred
%}

%token NUM EXIT
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
input:
      /* empty */
    | input line
    ;

line:
      expr '\n'  {
                    if (!error_flag)
                        printf("Result = %d\n", $1);
                    error_flag = false; // reset for next line
                 }
    | EXIT '\n'  { printf("Exiting...\n"); exit(0); }
    | '\n'
    ;

expr:
      expr '+' expr    { $$ = $1 + $3; }
    | expr '-' expr    { $$ = $1 - $3; }
    | expr '*' expr    { $$ = $1 * $3; }
    | expr '/' expr    {
                          if ($3 == 0) {
                              yyerror("Division by zero!");
                              error_flag = true;
                              $$ = 0;  // placeholder (won’t be printed)
                          } else {
                              $$ = $1 / $3;
                          }
                       }
    | '-' expr %prec UMINUS { $$ = -$2; }
    | '(' expr ')'     { $$ = $2; }
    | NUM              { $$ = $1; }
    ;
%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

int main() {
    printf("Enter an arithmetic expression (type 'exit' or '$' to quit):\n");
    yyparse();
    return 0;
}
