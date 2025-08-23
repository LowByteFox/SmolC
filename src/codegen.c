/* SPDX-License-Identifier: BSD-2-Clause */
#include <ast.h>
#include <codegen.h>
#include "registers.h"
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
        char op_c = op->u.operation.op;
        const char *reg1 = gen_op(ctx, op->u.operation.left);
        const char *reg2 = gen_op(ctx, op->u.operation.right);

        switch (op_c) {
        case '+':
            str_builder_printf(&ctx->builder, "add %s, %s\n", reg1, reg2);
            break;
        case '-':
            str_builder_printf(&ctx->builder, "sub %s, %s\n", reg1, reg2);
            break;
        case '*': /* RDX:RXA := RXA * OP1 */
        case '/': /* RDX:RXA / OP1 => RAX (res), RDX (rem) */
        case '%':
        {
            const char *rax_back = NULL;
            const char *rdx_back = NULL;
            bool is_reg1_rax = strcmp(reg1, "rax") == 0;
            bool is_rdx_used = is_register_in_use("rdx");

            if (!is_reg1_rax) {
                rax_back = alloc_register_except((const char*[]) {"rax",
                    "rdx"}, 2);
                str_builder_printf(&ctx->builder, "mov %s, rax\n", rax_back);
                str_builder_printf(&ctx->builder, "mov rax, %s\n", reg1);
            }

            if (is_rdx_used) {
                rdx_back = alloc_register_except((const char*[]) {"rax",
                    "rdx"}, 2);
                str_builder_printf(&ctx->builder, "mov %s, rdx\n", rdx_back);
                str_builder_append_cstr(&ctx->builder, "xor rdx, rdx\n");
            }

            if (op_c == '*')
                str_builder_printf(&ctx->builder, "mul %s\n", reg2);
            else if (op_c == '/' || op_c == '%') {
                if (!is_rdx_used)
                    str_builder_append_cstr(&ctx->builder, "xor rdx, rdx\n");
                str_builder_printf(&ctx->builder, "div %s\n", reg2);
            }

            if (!is_reg1_rax) {
                if (op_c == '*' || op_c == '/')
                    str_builder_printf(&ctx->builder, "mov %s, rax\n", reg1);
                else if (op_c == '%')
                    str_builder_printf(&ctx->builder, "mov %s, rdx\n", reg1);
                str_builder_printf(&ctx->builder, "mov rax, %s\n", rax_back);
                free_register(rax_back);
            } else {
                if (op_c == '%')
                    str_builder_printf(&ctx->builder, "mov %s, rdx\n", reg1);
            }

            if (is_rdx_used) {
                str_builder_printf(&ctx->builder, "mov rdx, %s\n", rdx_back);
                free_register(rdx_back);
            }
            break;
        }
        // case '/': {
        //     if (strcmp(reg1, "rax") == 0) {
        //         const char *tmp = alloc_register_except("rdx");
        //         str_builder_printf(&ctx->builder, "mov %s, rdx\n", tmp);
        //         str_builder_append_cstr(&ctx->builder, "xor rdx, rdx\n");
        //         str_builder_printf(&ctx->builder, "div %s\n", reg2);
        //         str_builder_printf(&ctx->builder, "mov rdx, %s\n", tmp);
        //         free_register(tmp);
        //     } else {
        //         const char *tmp = alloc_register_except("rdx");
        //         const char *tmp2 = alloc_register_except("rdx");
        //         str_builder_printf(&ctx->builder, "mov %s, rax\n", tmp);
        //         str_builder_printf(&ctx->builder, "mov %s, rdx\n", tmp2);
        //         str_builder_append_cstr(&ctx->builder, "xor rdx, rdx\n");
        //         str_builder_printf(&ctx->builder, "mov rax, %s\n", reg1);
        //         str_builder_printf(&ctx->builder, "div %s\n", reg2);
        //         str_builder_printf(&ctx->builder, "mov %s, rax\n", reg1);
        //         str_builder_printf(&ctx->builder, "mov rax, %s\n", tmp);
        //         str_builder_printf(&ctx->builder, "mov rdx, %s\n", tmp2);
        //         free_register(tmp);
        //         free_register(tmp2);
        //     }
        //     break;
        // }
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
