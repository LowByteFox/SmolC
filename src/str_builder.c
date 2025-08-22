/* SPDX-License-Identifier: BSD-2-Clause */
#include <stdarg.h>
#include <stdio.h>
#include <str_builder.h>

#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 32

static void expand_buffer(struct str_builder *b);

void str_builder_deinit(struct str_builder *b)
{
    free(b->buffer);
    b->allocated = 0;
}

void str_builder_shrink(struct str_builder *b)
{
    if (b->buffer != NULL) {
        b->allocated = b->len;
        b->buffer= realloc(b->buffer, b->allocated);
    }
}

char *str_builder_str(struct str_builder *b)
{
    char *ptr = 0;

    if (b->buffer != NULL) {
        ptr = b->buffer;
        b->buffer = NULL;
        b->len = 0;
        b->allocated = 0;
    }

    return ptr;
}

void str_builder_append_char(struct str_builder *b, char c)
{
    while (b->len + 1 >= b->allocated)
        expand_buffer(b);

    b->buffer[b->len] = c; 
    b->len++;
    b->buffer[b->len] = 0;
}

void str_builder_append_cstr(struct str_builder *b, const char *str)
{
    int len = strlen(str);
    while (b->len + len + 1 >= b->allocated)
        expand_buffer(b);

    memcpy(b->buffer + b->len, str, len);
    b->len += len;
    b->buffer[b->len] = 0;
}

void str_builder_printf(struct str_builder *b, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    size_t length = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    va_start(args, fmt);
    char buf[length + 1];
    memset(buf, 0, length + 1);
    vsnprintf(buf, length + 1, fmt, args);
    str_builder_append_cstr(b, buf);
    va_end(args);
}


static void expand_buffer(struct str_builder *b)
{
    if (b->buffer != NULL) {
        b->allocated *= 2;
        b->buffer = realloc(b->buffer, b->allocated * sizeof(char));
    } else {
        b->allocated = INIT_SIZE;
        b->buffer = calloc(b->allocated, sizeof(char));
    }
}
