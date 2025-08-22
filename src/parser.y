/* SPDX-License-Identifier: BSD-2-Clause */
%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); }

int result;  // store the final result
%}

%token NUMBER
%token PLUS

%%

expr: term
    | expr PLUS term { result = $1 + $3; }
    ;

term: NUMBER { $$ = $1; }
    ;

%%
