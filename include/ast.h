/* SPDX-License-Identifier: BSD-2-Clause */
#ifndef AST_H
#define AST_H

#include <stdint.h>

#define _(name) struct ast *name##_node

enum smolc_ast_type {
    PROGRAM,
    INT,
    OP,
};

struct ast {
    enum smolc_ast_type tag;
    union {
        struct ast *program;
        uint64_t number;
        struct {
            struct ast *left;
            struct ast *right;
            char op;
        } operation;
    } u;
};

struct ast *parse(void);

_(program)(struct ast *node);
_(int)(uint64_t num);
_(op)(char op, struct ast *left, struct ast *right);

_(debug)(struct ast *node);

#undef _

#endif
