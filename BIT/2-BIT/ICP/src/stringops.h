// Subor: stringops.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Operacie s retazcami

#ifndef STROPS_H
#define STROPS_H

#include <string>

#include "vectorops.h"

class StrOps
{
public:
    static std::vector<std::string> split(std::string s, const char delimiter = ',');
    static int toInt(std::string s);
    static std::string iToStr(int i);

private:
    StrOps();
    ~StrOps();
};

#endif // STROPS_H
