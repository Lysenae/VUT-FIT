// Subor: tilerotation.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda rotacie dlazdice

#ifndef TILEROTATION_H
#define TILEROTATION_H

class TileRotation
{
public:
    explicit TileRotation(unsigned int rotation);
    ~TileRotation();
    void next();
    unsigned int val() const;
    void operator=(unsigned int rotation);

private:
    unsigned int _rotation;
};

#endif // TILEROTATION_H
