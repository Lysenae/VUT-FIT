// Subor: stringops.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda dlazdice

#ifndef TILE_H
#define TILE_H

#include <string>
#include <iostream>

#include "tilerotation.h"
#include "objectives.h"
#include "vectorops.h"

typedef enum tTileType
{
    Undef = 0,
    T_Tile,
    L_Tile,
    I_Tile
} TileType;

class Tile
{
public:
    Tile(TileType type, int position, unsigned int rotation, std::vector<int> players, int objective);
    virtual ~Tile();

    // Gettery a settery
    // Niektore settery v GUI dodatocne emituju signaly o zmene

    // Hodnota v *_rotation
    unsigned int rotation() const;

    // _type
    TileType tileType() const;

    // _position
    int position() const;
    void setPosition(int position);

    // _loot
    int loot() const;
    virtual void setLoot(int loot);            // Virtualna
    bool hasLoot() const;

    // _players
    std::vector<int> playerList() const;
    std::vector<int>& playerListRef();
    virtual void addPlayer(int player);        // Virtualna
    virtual void removePlayer(int player);     // Virtualna

    // _active
    bool active() const;
    virtual void setActive(bool active);       // Virtualna

    // _rotatable
    bool rotatable() const;
    void setRotatable(bool rotatable);

    // _path_*
    bool pointsUp();
    bool pointsRight();
    bool pointsDown();
    bool pointsLeft();

    void rotate(unsigned int val);

    std::vector<std::string> toString(int loot) const;
    void print(int pLoot) const;
    void rotationHint() const;

    static const int STR_SIZE = 5;
    static const int TILE_WIDTH = 7;

private:
    TileRotation *_rotation;

    TileType _type;
    int      _position;
    int      _loot;
    bool     _active;
    bool     _rotatable;

    std::vector<int> _players;

    // Mozne cesty
    bool _pathTop;
    bool _pathRight;
    bool _pathBottom;
    bool _pathLeft;

    // Tieto metody nie su pristupne GUI verzii
    std::string symbolByRotation() const;
    std::string lootToString(int pLoot) const;
    std::string playersToString() const;
    std::string activeToString() const;
    std::string posToString() const;

protected:
    void setPaths();
    void rotationNext();
    void sortVector(std::vector<int> &vec);
};

#endif // TILE_H
