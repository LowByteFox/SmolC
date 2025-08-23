/* SPDX-License-Identifier: BSD-2-Clause */
%{
#include "y.tab.h"
%}
%option yylineno

%%

 /* Brackets */
"("         return '(';
")"         return ')';

 /* Operators */
"+"         return '+';
"-"         return '-';
"*"         return '*';
"/"         return '/';
"%"         return '%';

 /* Values */
[0-9]+      { yylval.num = atol(yytext); return INT_TOK;        }
[ \t\n]     ;

 /* Dead End */
.           { fprintf(stderr, "???: %s\n", yytext); abort();    }

%%

int yywrap(void) { return 1; }
