// error.c
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Chybove hlasenia

#include "error.h"

void warning_msg(const char *fmt, ...)
{
  fprintf(stderr, "CHYBA: ");
  va_list vargs;
  va_start(vargs, fmt);
  vfprintf(stderr, fmt, vargs);
  va_end(vargs);
}

void fatal_error(const char *fmt, ...)
{
  fprintf(stderr, "CHYBA: ");
  va_list vargs;
  va_start(vargs, fmt);
  vfprintf(stderr, fmt, vargs);
  va_end(vargs);
  exit(1);
}