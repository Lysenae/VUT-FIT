// Subor: objectives.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Triede reprezentujuca mnozinu uloh.

#ifndef OBJECTIVES_H
#define OBJECTIVES_H

#include <stdexcept>
#include <iostream>

#include "random.h"
#include "vectorops.h"

class Objectives
{
public:
    Objectives(int size);
    Objectives(std::vector<int> objectives);
    ~Objectives();

    int remaining() const;
    int next();
    void add(int objective);
    int giveBack(int objective);
    std::vector<int> objectiveList() const;

    static std::string toString(int objective);

private:
    std::vector<int> _objectives;
};

#endif // OBJECTIVES_H
