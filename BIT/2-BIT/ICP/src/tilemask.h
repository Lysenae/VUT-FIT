// Subor: tilemask.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda masky dlazdic

#ifndef TILEMASK_H
#define TILEMASK_H

#include <iostream>

#include "tileset.h"
#include "random.h"
#include "vectorops.h"

class TileMaskItem;

class TileMask
{
public:
    explicit TileMask();
    explicit TileMask(int size, int &t_tiles, int &l_tiles, int &i_tiles, int players);
    explicit TileMask(TileSet *ts, Tile *ft = NULL);
    ~TileMask();
    TileMaskItem& operator[](unsigned int i);
    TileMaskItem& last();
    int size() const;
    void copy(unsigned int i, unsigned int j);
    void set(unsigned int i, TileType type, unsigned int rot, int pos,
        std::vector<int> players, int objective);
    void addItem(TileType type, unsigned int rot, int pos,
        std::vector<int> players, int objective);
    std::vector<int> findAvailable(int size) const;
    std::string itemToString(unsigned int index) const;
    bool empty() const;

private:
    std::vector<TileMaskItem> _items;

    bool even(int i) const;
    unsigned int avaliableTypes(int t, int l, int i) const;
    TileType getTileType(int idx, unsigned int n, int &t, int &l, int &i);
};

class TileMaskItem
{
public:
    explicit TileMaskItem(TileType type, unsigned int rotation,
        int position, std::vector<int> players, int objective);
    ~TileMaskItem();

    TileType type() const;
    void setType(TileType type);
    unsigned int rotation() const;
    void setRotation(unsigned int rotation);
    int position() const;
    void setPosition(int position);
    std::vector<int> players() const;
    void setPlayers(std::vector<int> players);
    void addPlayer(int player);
    int objective() const;
    void setObjective(int objective);

private:
    unsigned int     _rotation;
    TileType         _type;
    int              _position;
    std::vector<int> _players;
    int              _objective;
};

#endif // TILEMASK_H
