#include "recursive.h"
#include "timeout.h"

unsigned fib(unsigned n)
{
    if (timeout) return 0;

    if (n <= 1)
        return n;
    else
        return fib(n-1) + fib(n-2);
}

unsigned ackermann(unsigned m, unsigned n)
{
    if (timeout) return 0;

    if (m == 0)
        return n+1;
    if (n == 0)
        return ackermann(m-1, 1);
    
    return ackermann(m-1, ackermann(m, n-1));
}


