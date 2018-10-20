// Subor: boardmodel.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: BoardModel predstavuje hraciu dosku labyrintu.
//        Je to zakladna triedy programu, ktora riadi celu logiku a
//        vyuziva ostatne moduly.

#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <iostream>
#include <fstream>

#include "tile.h"
#include "tileset.h"
#include "tilemask.h"
#include "players.h"
#include "objectives.h"
#include "reversestep.h"
#include "random.h"
#include "vectorops.h"
#include "stringops.h"

enum ShiftType
{
    Shift_None = 0,
    Shift_Down,
    Shift_Up,
    Shift_Right,
    Shift_Left,
};

class BoardModel
{
public:
    BoardModel();
    ~BoardModel();
    virtual void createGame();
    virtual int run(TileMask tmask);
    virtual void load(std::string fName);

private:
    int _goal;
    int _size;
    int _l_tiles;
    int _t_tiles;
    int _i_tiles;
    int _phase;
    int _forbidden;

    Tile       *_free_tile;
    TileSet    *_tiles;
    Players    *_players;
    Objectives *_objectives;

    std::vector<ReverseStep> _rSteps;

    void printScores() const;

protected:
    // Gettery a settery pre privatne premenne

    // _goal
    int goal() const;
    void setGoal(int goal);

    // _size
    int boardSize() const;
    void setBoardSize(int size);

    // _players
    Players* players() const;
    void setPlayers(Players* players);
    void freePlayers();

    // _objectives
    Objectives* objectives() const;
    void setObjectives(Objectives *objectives);
    void freeObjectives();

    // pocty typov dlazdic (_t_tiles, _l_tiles, _i_tiles)
    void tileCounters(int &t_tiles, int &l_tiles, int &i_tiles) const;
    void setTilesCount();

    // _phase
    int phase() const;
    virtual void switchPhase();
    void setPhase(int phase);

    // _forbidden
    int forbidden() const;
    void setForbidden(int forbidden);

    // *_tiles
    TileSet* tileSet();

    // *_free_tile
    Tile* freeTile();
    void setFreeTile(Tile *tile);

    std::vector<ReverseStep>& reverseSteps();

    // Ine metody
    void initializeObjectives(TileMask &tm);
    std::vector<int> findShiftable();
    int calculateOpposite(int pos, ShiftType &st);
    TileMask shiftTiles(int pos);
    std::vector<int> findPath(int pos);
    void moveTo(int pos, bool canPickUp = true);
    void activateTiles(std::vector<int> tileIdxs);
    void spawnObjective(int objective, int position = -1);
    void removeObjective(int objective);
    TileMask applyReverseStep();

    // Virtualne metody
    virtual void showBoard(TileMask &tm);
    virtual void save(std::string fName);
};

#endif // BOARDMODEL_H
