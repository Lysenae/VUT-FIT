// ppm.h
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Praca s formatom .ppm

#ifndef PPM_H
#define PPM_H

#include "error.h"

#define WRITE_OK 0
#define WRITE_FAIL 1
#define COLBYTES 3

struct ppm
{
  unsigned xsize;
  unsigned ysize;
  char data[];    // RGB bajty, 3*xsize*ysize
};

struct ppm *ppm_read(const char *filename);
int ppm_write(struct ppm *p, const char *filename);
unsigned long ppm_size(struct ppm *p);
void ppm_free(struct ppm *p);

#endif // PPM_H