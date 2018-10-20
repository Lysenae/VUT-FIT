// Subor: reversestep.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda reprezentujuca reverzne kroky pre Undo

#include "reversestep.h"

/// Konstruktor.
///
/// @param rsType Type kroku.
/// @param sf Index dlazdice, ktoru je zakazane posuvat.
/// @param sp Index dlazdice protilahlej od, ktorej sa posuvalo.
/// @param mp Pozicia, z ktorej sa hrac posuval.
/// @param pl Cislo ulohy, ktora bola v kroku splnena.
ReverseStep::ReverseStep(int rsType, int sf, int sp, int mp, int pl)
{
    type           = rsType;
    shiftForbidden = sf;
    shiftPos       = sp;
    movePos        = mp;
    pickedLoot     = pl;
}

/// Destruktor.
ReverseStep::~ReverseStep()
{
}

