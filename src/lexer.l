/* SPDX-License-Identifier: BSD-2-Clause */
%{
#include "y.tab.h"
%}

%%

[0-9]+      { yylval = atoi(yytext); return NUMBER; }
[ \t\n]+    ;  /* skip whitespace */
"+"         { return PLUS; }

.           { return yytext[0]; } /* fallback for unexpected chars */

%%
int yywrap(void) { return 1; }
