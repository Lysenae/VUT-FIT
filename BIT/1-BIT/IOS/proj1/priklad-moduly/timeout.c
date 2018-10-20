#include <signal.h>
#include <unistd.h>
#include "timeout.h"

volatile int timeout = 0;

void catch_sig_alarm(int sig)
{
    (void)sig;
    timeout = 1;
}

void set_timeout(unsigned seconds)
{
    signal(SIGALRM, catch_sig_alarm);
    timeout = 0;
    alarm(seconds);
}

