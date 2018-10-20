// Subor: random.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Metody pre pracu s RNG.

#ifndef RANDOM_H
#define RANDOM_H

#include <vector>
#include <algorithm>
#include <random>

// Modul vyzaduje standard C++11

class Random
{
public:
    static unsigned int random(unsigned int low, unsigned int high);
    static void shuffle(std::vector<int> &vec);

private:
    Random();
    ~Random();
};

#endif // RANDOM_H
