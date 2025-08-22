/* SPDX-License-Identifier: BSD-2-Clause */
#include <ast.h>

int main(void)
{
    struct ast *parsed = parse();
    debug_node(parsed);
    return 0;
}
