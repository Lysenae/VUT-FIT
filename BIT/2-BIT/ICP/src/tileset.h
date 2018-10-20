// Subor: tilerotation.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda reprezentujuca mnozinu dlazdic v hracej ploche


#ifndef TILESET_H
#define TILESET_H

#include <vector>

#include "tile.h"

class TileSet
{
public:
    explicit TileSet();
    ~TileSet();
    int size();
    void empty();
    void append(Tile *tile);
    Tile* operator[](unsigned int i);
    Tile* at(unsigned int i);
    void set(unsigned int i, Tile* tile);
    std::vector<int> findAvailable(int size) const;

private:
    std::vector<Tile *> _tiles;
};

#endif // TILESET_H
