// error.h
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Chybove hlasenia

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h> // exit()
#include <stdarg.h>

void warning_msg(const char *fmt, ...);
void fatal_error(const char *fmt, ...);

#endif // ERROR_H