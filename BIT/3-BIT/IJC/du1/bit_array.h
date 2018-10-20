// bit_array.h
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Definicia typu bit_array_t a operacii na nim

#ifndef BITARRAY_H
#define BITARRAY_H

#include <stdio.h>

#include "error.h"

// Skrateny zapis unsigned long
typedef unsigned long ulong;
typedef ulong * bit_array_t;

// Pocet bitov ulong
#define ULONG_BITS (sizeof(ulong) * 8)
#define NO_VAL -1 // Vracia sa v pripade chyby, nemala by byt nikdy vratena

#define DU1_GET_BIT_(p, i) \
  ((p[i/ULONG_BITS+1] & (1UL << (i - ULONG_BITS * (i/ULONG_BITS)))) ? 1 : 0)

#define DU1_SET_BIT_(p, i, b) \
  (b == 0 ? \
    (p[i/ULONG_BITS+1] &= ~(1UL << (i - ULONG_BITS * (i/ULONG_BITS)))) : \
    (p[i/ULONG_BITS+1] |= (1UL << (i - ULONG_BITS * (i/ULONG_BITS)))))

#define ba_create(ba_name, ba_size) \
  ulong ba_name[ba_size / ULONG_BITS + 2] = { ba_size, }

#ifndef USE_INLINE

  #define ba_size(ba_name) \
    ba_name[0]

  #define ba_set_bit(ba_name, ba_index, ba_value) \
    ((ba_index >= ba_size(ba_name)) ? \
      (fatal_error("Index %ld mimo rozsah 0..%ld", \
        (long)ba_index, (long)(ba_size(ba_name)-1)), NO_VAL) : \
      DU1_SET_BIT_(ba_name, ba_index, ba_value))

  #define ba_get_bit(ba_name, ba_index) \
    ((ba_index >= ba_size(ba_name)) ? \
      (fatal_error("Index %ld mimo rozsah 0..%ld", \
        (long)ba_index, (long)(ba_size(ba_name)-1)), NO_VAL) : \
      DU1_GET_BIT_(ba_name, ba_index))

#else

  inline ulong ba_size(bit_array_t ba_name)
  {
    return ba_name[0];
  }

  inline void ba_set_bit(bit_array_t ba_name, ulong ba_index, int ba_value)
  {
    if(ba_index < ba_size(ba_name))
    {
      DU1_SET_BIT_(ba_name, ba_index, ba_value);
    }
    else
    {
      fatal_error("Index %ld mimo rozsah 0..%ld",
        (long)ba_index, (long)(ba_size(ba_name)-1));
    }
  }

  inline int ba_get_bit(bit_array_t ba_name, ulong ba_index)
  {
    if(ba_index < ba_size(ba_name))
    {
      return DU1_GET_BIT_(ba_name, ba_index);
    }
    else
    {
      fatal_error("Index %ld mimo rozsah 0..%ld",
        (long)ba_index, (long)(ba_size(ba_name)-1));
      return NO_VAL;
    }
  }

#endif // USE_INLINE

#endif // BITARRAY_H
