/* SPDX-License-Identifier: BSD-2-Clause */
#ifndef STRBUILDER_H
#define STRBUILDER_H

#include <stddef.h>

struct str_builder {
    char *buffer;
    size_t len;
    size_t allocated;
};

void str_builder_deinit(struct str_builder *b);
void str_builder_shrink(struct str_builder *b);
char *str_builder_str(struct str_builder *b);
void str_builder_append_char(struct str_builder *b, char c);
void str_builder_append_cstr(struct str_builder *b, const char *str);
void str_builder_printf(struct str_builder *b, const char *fmt, ...);

#endif
