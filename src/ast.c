/* SPDX-License-Identifier: BSD-2-Clause */
#include <ast.h>

#include <stdio.h>
#include <stdlib.h>

#define _(name) struct ast *name##_node
#define __(body) {\
    struct ast *node = malloc(sizeof(*node));\
    body\
    return node;\
}

#ifndef _WIN32
#define CLEAR   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#else
#error "Wait for Windows support!"
#endif

static void offset_by(int count);
static void _debug_node(struct ast *node, int spacing);

_(program)(struct ast *parent)
__({
    node->tag = PROGRAM;
    node->u.program = parent;
})

_(int)(uint64_t num)
__({
    node->tag = INT;
    node->u.number = num;
})

_(op)(char op, struct ast *left, struct ast *right)
__({
    node->tag = OP;
    node->u.operation.op = op;
    node->u.operation.left = left;
    node->u.operation.right = right;
})

static void offset_by(int count)
{
    for (int i = 0; i < count; i++)
        putchar(' ');
}

static void _debug_node(struct ast *node, int spacing)
{
    if (node == NULL) {
        offset_by(spacing);
        printf("null\n");
        return;
    }

    switch (node->tag) {
    case PROGRAM:
        offset_by(spacing);
        printf(CYAN"Program"CLEAR" {\n");
        _debug_node(node->u.program, spacing + 2);
        offset_by(spacing);
        printf("}\n");
        break;
    case INT:
        offset_by(spacing);
        printf(YELLOW"Int"CLEAR": %ld\n", node->u.number);
        break;
    case OP:
        offset_by(spacing);
        printf(PURPLE"Operation"CLEAR": %c {\n", node->u.operation.op);
        _debug_node(node->u.operation.left, spacing + 2);
        _debug_node(node->u.operation.right, spacing + 2);
        offset_by(spacing);
        printf("}\n");
        break;
      break;
    }
}

_(debug)(struct ast *node)
{
    _debug_node(node, 0);
    return node;
}
