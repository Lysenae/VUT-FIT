// primes.c
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Testovanie Eratosthenovho sita

#include <stdio.h>

#include "bit_array.h"
#include "eratosthenes.h"

#define BA_SIZE 202000000

int main()
{
  ba_create(ba, BA_SIZE);
  Eratosthenes(ba);

  ulong primes_found   = 0;
  ulong last_prime_idx = BA_SIZE-1;

  // Vyhladanie poslednych 10 prvocisel
  for(ulong i=last_prime_idx; i>=0; i--)
  {
    if(ba_get_bit(ba, i) == 0)
    {
      primes_found++;
      last_prime_idx = i;
    }

    if(primes_found == 10)
    {
      break;
    }
  }

  // Vypis poslednych 10 prvocisel
  for(ulong i=last_prime_idx; i<BA_SIZE; i++)
  {
    if(ba_get_bit(ba, i) == 0)
    {
      printf("%lu\n", i);
    }
  }

  return 0;
}