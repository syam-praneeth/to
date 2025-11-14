%{
#include "calc.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%
[0-9]+          { yylval = atoi(yytext); return NUM; }
[ \t\r]+        ;                /* ignore spaces/tabs */
"exit"          return EXIT;
"$"             return EXIT;
"("             return '(';
")"             return ')';
[\+\-\*/]       { return yytext[0]; }
\n              return '\n';
.               { return yytext[0]; }
%%

int yywrap() { return 1; }
