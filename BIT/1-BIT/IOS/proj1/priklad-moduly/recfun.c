/**
 * Priklad pro:
 *
 *      - modularni programovani
 *      - rekurzivni funkci (prima rekurze)
 *      - jednoduche zachytavani asynchronniho signalu
 *
 * Pro plnou funkcionalitu vyzkousejte:
 *
 *  $ make
 *  $ ./recfun -f 10
 *  $ ./recfun -f 42            // timeout
 *  $ ./recfun -t 30 -f 42      // takes ~6s on i7-3770K
 *
 *  $ ./recfun -a 3,4
 *  $ ./recfun -a 4,1           // timeout
 *  $ ./recfun -t 90 -a 4,1     // takes ~17s on i7-3770K
 *
 * Puvodni ucel jako demonstracni priklad pro 1. ulohu IOS 2013/14:
 *      - zobrazeni zavislosti zdrojovych souboru
 *      - hledani a zobrazeni zavislosti symbolu na zdrojovych kodech
 *      - hledani a zobrazeni grafu volani funkci (z binarniho souboru)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "timeout.h"
#include "print_recursive.h"

void usage(const char *message)
{
    if (message)
        fprintf(stderr, "error: %s\n", message);

    fprintf(stderr,
            "syntax: \n"
            "   printr [-t SECONDS] -f N\n"
            "       print the Fibonnaci sequence\n"
            "   printr [-t SECONDS] -a M,N\n"
            "       print the value of Ackermann function\n"
            "option:\n"
            "   -t sets the timeout for execution (default: 2 seconds)\n");
}

void failure(const char *s)
{
    usage(s);
    exit(EXIT_FAILURE);
}

unsigned strton(const char *s, const char *error_msg)
{
    unsigned result;
    char *endptr;
    result = strtoul(s, &endptr, 10);
    if (*endptr)
        failure(error_msg);
    return result;
}


int main(int argc, char **argv)
{
    unsigned timeoutvalue = 2;
    unsigned m, n;
    char *nptr; // for parsing ackermann pair of values

    enum { F_NONE, F_FIB, F_ACK } funflag = F_NONE;

    int c;

    while ((c = getopt(argc, argv, "t:f:a:")) != -1)
    {
        switch (c)
        {
            case 't':
                timeoutvalue = strton(optarg, "invalid timeout value");
                break;
            case 'f':
                if (funflag)
                    failure("multiple function requested");
                funflag = F_FIB;
                n = strton(optarg, "invalid value for Fibanacci function");
                break;
            case 'a':
                if (funflag)
                    failure("multiple function requested");
                funflag = F_ACK;
                nptr = strchr(optarg, ',');
                if (nptr)
                    // ugly, but working for this trivial example
                    *nptr++ = '\0'; // set string STOP instead of ','
                else 
                    failure("invalid pair of values for Ackermann function");
                m = strton(optarg, "invalid M value for Ackermann function");
                n = strton(nptr, "invalid N value for Ackermann function");
                break;
            default:
                failure("invalid arguments");
        }
    }

    set_timeout(timeoutvalue);
    switch(funflag)
    {
        case F_NONE:
            failure("function not requested");
        case F_FIB:
            print_fib_upto(n);
            break;
        case F_ACK:
            print_ackermann(m, n);
            break;
    }
    return EXIT_SUCCESS;
}
