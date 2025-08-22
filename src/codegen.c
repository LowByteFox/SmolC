/* SPDX-License-Identifier: BSD-2-Clause */
#include <ast.h>
#include <codegen.h>
#include <registers.h>
#include <str_builder.h>

#include <stdlib.h>
#include <string.h>

struct context {
    struct str_builder builder;
};

static const char *gen_op(struct context *ctx, struct ast *op);

char *codegen(struct ast *ast)
{
    struct context ctx = { 0 };
    gen_op(&ctx, ast->u.program);

    return ctx.builder.buffer;
}


static const char *gen_op(struct context *ctx, struct ast *op)
{
    switch (op->tag) {
    case INT: {
        const char *reg = alloc_register();
        str_builder_printf(&ctx->builder, "mov %s, %ld\n", reg, op->u.number);
        return reg;
    }
    case OP: {
        const char *reg1 = gen_op(ctx, op->u.operation.left);
        const char *reg2 = gen_op(ctx, op->u.operation.right);

        switch (op->u.operation.op) {
        case '+':
            str_builder_printf(&ctx->builder, "add %s, %s\n", reg1, reg2);
            break;
        case '-':
            str_builder_printf(&ctx->builder, "sub %s, %s\n", reg1, reg2);
            break;
        case '*': {
            if (strcmp(reg1, "rax") == 0) {
                str_builder_printf(&ctx->builder, "mul %s\n", reg2);
            } else {
                const char *tmp = alloc_register_except("rdx");
                str_builder_printf(&ctx->builder, "mov %s, rax\n", tmp);
                str_builder_printf(&ctx->builder, "mov rax, %s\n", reg1);
                str_builder_printf(&ctx->builder, "mul %s\n", reg2);
                str_builder_printf(&ctx->builder, "mov %s, rax\n", reg1);
                str_builder_printf(&ctx->builder, "mov rax, %s\n", tmp);
                free_register(tmp);
            }
            break;
        }
        case '/': {
            if (strcmp(reg1, "rax") == 0) {
                const char *tmp = alloc_register_except("rdx");
                str_builder_printf(&ctx->builder, "mov %s, rdx\n", tmp);
                str_builder_append_cstr(&ctx->builder, "xor rdx, rdx\n");
                str_builder_printf(&ctx->builder, "div %s\n", reg2);
                str_builder_printf(&ctx->builder, "mov rdx, %s\n", tmp);
                free_register(tmp);
            } else {
                const char *tmp = alloc_register_except("rdx");
                const char *tmp2 = alloc_register_except("rdx");
                str_builder_printf(&ctx->builder, "mov %s, rax\n", tmp);
                str_builder_printf(&ctx->builder, "mov %s, rdx\n", tmp2);
                str_builder_append_cstr(&ctx->builder, "xor rdx, rdx\n");
                str_builder_printf(&ctx->builder, "mov rax, %s\n", reg1);
                str_builder_printf(&ctx->builder, "div %s\n", reg2);
                str_builder_printf(&ctx->builder, "mov %s, rax\n", reg1);
                str_builder_printf(&ctx->builder, "mov rax, %s\n", tmp);
                str_builder_printf(&ctx->builder, "mov rdx, %s\n", tmp2);
                free_register(tmp);
                free_register(tmp2);
            }
            break;
        }
        default:
            abort();
        }

        free_register(reg2);
        return reg1;
    }
    case BRACKETS:
        return gen_op(ctx, op->u.brackets);
        break;
    default:
        abort();
    }

    return 0;
}
