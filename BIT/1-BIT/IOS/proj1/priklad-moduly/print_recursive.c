#include <stdio.h>
#include "print_recursive.h"
#include "recursive.h"
#include "timeout.h"

void print_fib_upto(unsigned n)
{
    printf("Fibonnacci sequence:");
    unsigned f;
    for (unsigned i = 0; i <= n; i++)
    {
        f = fib(i);
        if (timeout)
        {
            printf(" !timeout\n");
            return;
        }
        printf(" %u", f);
        fflush(stdout);
    }
    printf("\n");
}

void print_ackermann(unsigned m, unsigned n)
{
    printf("Ackermann function: A(%u,%u) = ", m, n);
    fflush(stdout);
    unsigned ack = ackermann(m, n);
    if (timeout)
        printf("!timeout\n");
    else
        printf("%u\n", ack);
}
