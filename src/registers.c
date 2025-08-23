/* SPDX-License-Identifier: BSD-2-Clause */
#include "registers.h"

#include <stdbool.h>
#include <string.h>

struct reg {
    const char *name;
    bool in_use;
};

static struct reg registers[] = {
    {"rax", false},
    {"rdi", false},
    {"rsi", false},
    {"rdx", false},
    {"rcx", false},
    {"r8",  false},
    {"r9",  false},
    {"r11", false},
    {0,     false},
};

const char *alloc_register(void)
{
    struct reg *iter = registers;
    while (iter->name != 0) {
        if (!iter->in_use) {
            iter->in_use = true;
            return iter->name;
        }
        iter++;
    }

    return 0;
}

const char *alloc_register_except(const char *regs[], size_t count)
{
    struct reg *iter = registers;
    while (iter->name != 0) {
        if (!iter->in_use) {
            size_t i = 0;
            for (; i < count; i++) {
                if (strcmp(iter->name, regs[i]) == 0)
                    break;
            }

            if (i == count) {
                iter->in_use = true;
                return iter->name;
            }
        }
        iter++;
    }

    return 0;
}

bool is_register_in_use(const char *reg)
{
    struct reg *iter = registers;
    while (iter->name != 0) {
        if (strcmp(iter->name, reg) == 0)
            return iter->in_use;
            
        iter++;
    }

    return false;
}

void free_register(const char *reg)
{
    struct reg *iter = registers;
    while (iter->name != 0) {
        if (iter->in_use && strcmp(iter->name, reg) == 0) {
            iter->in_use = false;
            break;
        }
            
        iter++;
    }
}
