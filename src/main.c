/* SPDX-License-Identifier: BSD-2-Clause */
#include <stdio.h>

int yyparse(void);
extern int result;

int main()
{
    if (yyparse() == 0) {
        printf("Result: %d\n", result);
    }
    return 0;
}
