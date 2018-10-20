// Subor: vectorops.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Operacie s vektormi

#ifndef VECOPS_H
#define VECOPS_H

#include <vector>
#include <algorithm>

#include "stringops.h"

class VecOps
{
public:
    static std::string toString(std::vector<int> vec, bool noSpace = false);
    static void sort(std::vector<int> &vec);
    static void sort(std::vector<std::string> &vec);
    static void reverse(std::vector<int> &vec);
    static void reverse(std::vector<std::string> &vec);
    static std::vector<int> fromStringI(std::string str, const char delimiter = ',');

private:
    VecOps();
    ~VecOps();
};

#endif // VECOPS_H
