/* SPDX-License-Identifier: BSD-2-Clause */
#include <ast.h>
#include <codegen.h>

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *buffer = codegen(parse());
    puts(buffer);

    free(buffer);
    return 0;
}
