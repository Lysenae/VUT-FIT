// Subor: objectives.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Triede reprezentujuca mnozinu hracov.

#ifndef PLAYERS_H
#define PLAYERS_H

#include <vector>
#include <stdexcept>
#include <string>

#include <iostream>

class Players
{
public:
    Players(int count, int size);
    Players(int count, int current, std::vector<int> positions,
        std::vector<int> scores, std::vector<int> objectives);
    ~Players();

    void next();
    void previous();
    int count() const;
    int currentPosition() const;
    void updatePosition(int player, int position);
    int current() const;
    int currentScore() const;
    void incScore(int player);
    void decScore(int player);
    int score(int player) const;
    int currentObjective() const;
    void updateObjective(int player, int objective);

    // Pre ukladanie hry
    std::vector<int> positionList() const;
    std::vector<int> scoreList() const;
    std::vector<int> objectiveList() const;

    static std::string toString(int player);
    static std::string vectorToString(std::vector<int> vec);

private:
    int _current;
    int _count;

    std::vector<int> _scores;
    std::vector<int> _objectives;
    std::vector<int> _positions;
};

#endif // PLAYERS_H
