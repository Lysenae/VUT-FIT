// Subor: tilerotation.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda rotacie dlazdice

#include "tilerotation.h"

/// Konstruktor.
///
/// @param rotation Stupen otocenia (1-4)
TileRotation::TileRotation(unsigned int rotation)
{
    if(rotation > 0 && rotation < 5)
    {
        _rotation = rotation;
    }
    else
    {
        _rotation = 1;
    }
}

/// Destruktor.
TileRotation::~TileRotation()
{
}

/// Nasledujuca rotacia.
void TileRotation::next()
{
    if(_rotation == 4)
    {
        _rotation = 1;
    }
    else
    {
        _rotation += 1;
    }
}

/// Hodnota aktualnej rotacie.
///
/// @return Vrati hodnotu aktualnej rotacie.
unsigned int TileRotation::val() const
{
    return _rotation;
}

/// Operator priradenia.
///
/// Nastavi aktualnu hodnotu rotacie.
/// @param rotation Hodnota rotacie
void TileRotation::operator=(unsigned int rotation)
{
    _rotation = rotation;
}
