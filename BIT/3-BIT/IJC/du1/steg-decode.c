// steg-decode.c
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Testovanie praca s formatom .ppm a ziskanie skrytej spravy z obrazku

#include <ctype.h>

#include "ppm.h"
#include "eratosthenes.h"

#define LIM_SIZE 1000*1000*3
#define BITS 8

int main(int argc, char **argv)
{
  char c;
  char decoded = 0;
  int bit_idx = 0;

  if(argc != 2)
  {
    fatal_error("Nebol zadany vstupny obrazok\n");
  }

  struct ppm *pic = ppm_read(argv[1]);
  ulong psize     = ppm_size(pic);

  ba_create(ba, LIM_SIZE);
  Eratosthenes(ba);

  for(ulong i=0; i<psize; i++)
  {
    if(ba_get_bit(ba, i) == 0)
    {
      c = pic->data[i] & 1;
      (c == 1) ? (decoded |= 1 << bit_idx++) : (decoded &= ~(1 << bit_idx++));

      if(bit_idx == BITS)
      {
        bit_idx = 0;
        if(decoded == '\0')
        {
          break; // Koniec spravy
        }

        if(!isprint(decoded))
        {
          ppm_free(pic);
          fatal_error("Najdeny vadny znak v sprave\n");
        }

        putchar(decoded);
        decoded = 0;
      }
    }
  }

  putchar('\n');

  ppm_free(pic);

  return 0;
}