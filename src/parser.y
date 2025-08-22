/* SPDX-License-Identifier: BSD-2-Clause */
%{
#include <stdio.h>
#include <stdlib.h>
#include <ast.h>

extern int yylineno;
extern char *yytext;

int yylex(void);
int yyerror(const char *s);
static struct ast *root;
%}

%union {
    struct ast *node;
    uint64_t num;
}

%token <num> INT_TOK
%type <node> program expr

%left '+' '-'
%left '*' '/'

%%

program:
    expr                    { root = program_node($1);      }
    ;

expr:
    INT_TOK                 { $$ = int_node($1);            }
    | expr '+' expr         { $$ = op_node('+', $1, $3);    }
    | expr '-' expr         { $$ = op_node('-', $1, $3);    }
    | expr '*' expr         { $$ = op_node('*', $1, $3);    }
    | expr '/' expr         { $$ = op_node('/', $1, $3);    }
    | '(' expr ')'          { $$ = bracket_node($2);        }
    ;

%%

struct ast *parse() {
    yyparse();
    return root;
}

int yyerror(const char *s) {
    return fprintf(stderr, "Error at line (%d): %s: '%s'\n", yylineno, s, yytext);
}
