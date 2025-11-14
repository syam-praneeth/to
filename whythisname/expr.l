%{
#include "expr.tab.h"
%}

%%
[0-9]+      { return NUM; }
[a-zA-Z]+   { return ID; }
"("         { return '('; }
")"         { return ')'; }
[\+\-\*/]   { return yytext[0]; }
\n          { return 0; }      /* Stop on newline */
[ \t]       ;                  /* Ignore spaces */
.           { return yytext[0]; }
%%

int yywrap() { return 1; }
