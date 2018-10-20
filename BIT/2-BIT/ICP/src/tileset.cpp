// Subor: tilerotation.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda reprezentujuca mnozinu dlazdic v hracej ploche

#include "tileset.h"

/// Konstruktor.
TileSet::TileSet()
{
}

/// Destruktor.
TileSet::~TileSet()
{
}

/// Vyprazdni zoznam dlazdic.
void TileSet::empty()
{
    while(!_tiles.empty())
    {
        _tiles.pop_back();
    }
}

/// Prida dlazdicu do zoznamu dlazdic.
///
/// @param tile Ukazovatel na instanciu triedy Tile.
void TileSet::append(Tile *tile)
{
    _tiles.push_back(tile);
}

/// Vrati ukazovatel na dlazdicu na indexe i.
///
/// @param i Index dlazdice.
Tile* TileSet::at(unsigned int i)
{
    if(i < _tiles.size())
    {
        return _tiles[i];
    }

    return NULL;
}

/// Velkost zoznamu dlazdic.
///
/// @return Vrati velkost zoznamu dlazdic.
int TileSet::size()
{
    return _tiles.size();
}

/// Nastavi ukazovatel na i-tom indexe v zozname.
///
/// @param i Index.
/// @param tile Ukazovatel na instanciu triedy Tile.
void TileSet::set(unsigned int i, Tile* tile)
{
    if(i < _tiles.size())
    {
        _tiles[i] = tile;
    }
}
