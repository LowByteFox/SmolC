/* SPDX-License-Identifier: BSD-2-Clause */
#ifndef REGISTERS_H
#define REGISTERS_H

#include <stddef.h>
#include <stdbool.h>

const char *alloc_register(void);
const char *alloc_register_except(const char *reg[], size_t count);
bool is_register_in_use(const char *reg);
void free_register(const char *reg);

#endif
