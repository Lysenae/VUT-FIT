#ifndef __TIMEOUT_H
#define __TIMEOUT_H

// during some long execution,
// check this variable for positive value
extern volatile int timeout;

void set_timeout(unsigned seconds);

#endif
