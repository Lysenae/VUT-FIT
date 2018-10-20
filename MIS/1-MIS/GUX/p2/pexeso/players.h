// File:   players.h
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef PLAYER_H
#define PLAYER_H

#include <QVector>

class Players
{
public:
    Players();
    void setCount(int count);
    void next();
    int current();
    int count();
    int getScore(int i);
    void incScore(int i);
    void setScore(int i, int score);
    void setCurrent(int i);
    QVector<int> getWinners();

private:
    int m_count;
    int m_current;

    QVector<int> m_scores;
};

#endif // PLAYER_H
