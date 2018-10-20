// Subor: reversestep.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda reprezentujuca reverzne kroky pre Undo

#ifndef REVERSESTEP_H
#define REVERSESTEP_H

#include <iostream>

class ReverseStep
{
public:
    ReverseStep(int type, int sf, int sp, int mp, int pl);
    ~ReverseStep();

    int type;
    int shiftForbidden;
    int shiftPos;
    int movePos;
    int pickedLoot;
};

#endif // REVERSESTEP_H
