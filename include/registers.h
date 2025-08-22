/* SPDX-License-Identifier: BSD-2-Clause */
#ifndef REGISTERS_H
#define REGISTERS_H

const char *alloc_register(void);
const char *alloc_register_except(const char *reg);
void free_register(const char *reg);

#endif
