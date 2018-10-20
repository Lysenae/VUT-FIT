// eratosthenes.c
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Eratosthenovo sito

#include "eratosthenes.h"

void Eratosthenes(bit_array_t ba)
{
  ulong sqrt_max = (ulong)sqrt(ba_size(ba));
  ulong size = ba_size(ba);

  for(ulong i=2; i<sqrt_max; i++)
  {
    if(ba_get_bit(ba, i) == 0)
    {
      for(ulong j=i*2; j<size; j += i)
      {
        ba_set_bit(ba, j, 1);
      }
    }
  }

  // 0 a 1 nie su prvocisla
  if(size >= 1)
  {
    ba_set_bit(ba, 0, 1);
  }

  if(size >= 2)
  {
    ba_set_bit(ba, 1, 1);
  }
}
