// File:   players.cpp
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "players.h"

///
/// \brief Constructor.
///
Players::Players()
{
    setCount(1);
}

///
/// \brief Set player count.
/// \param count
///
void Players::setCount(int count)
{
    if(count < 1 || count > 4)
        m_count = 1;
    else
        m_count = count;
    m_current = 1;

    m_scores.clear();
    for(int i=0; i<m_count; ++i)
        m_scores.append(0);
}

///
/// \brief Set current player to next one.
///
void Players::next()
{
    m_current = (m_current + 1 > m_count) ? 1 : m_current + 1;
}

///
/// \brief Get current player.
/// \return Current player
///
int Players::current()
{
    return m_current;
}

///
/// \brief Get player count.
/// \return Player count
///
int Players::count()
{
    return m_count;
}

///
/// \brief Get score of player number i.
/// \param i Player number
/// \return Score
///
int Players::getScore(int i)
{
    i = i-1;
    if(i >=0 && i<m_count)
        return m_scores[i];
    return -1;
}

///
/// \brief Increase score of player i.
/// \param i Player number
///
void Players::incScore(int i)
{
    i = i-1;
    if(i >=0 && i<m_count)
    {
        int tmp = m_scores[i]+1;
        m_scores.replace(i, tmp);
    }
}

///
/// \brief Set score of player i.
/// \param i Player number
/// \param score
///
void Players::setScore(int i, int score)
{
    i = i-1;
    if(i >=0 && i<m_count)
    {
        m_scores.replace(i, score);
    }
}

///
/// \brief Set current player
/// \param i Player number
///
void Players::setCurrent(int i)
{
    if(i >0 && i<=m_count)
    {
        m_current = i;
    }
}

///
/// \brief Get player(s) with highest score.
/// \return Vector of player numbers
///
QVector<int> Players::getWinners()
{
    QVector<int> w;
    int max = 0;

    for(int i=0; i<m_scores.size(); ++i)
    {
        if(m_scores[i] > max)
            max = m_scores[i];
    }

    for(int i=0; i<m_scores.size(); ++i)
    {
        if(m_scores[i] == max)
            w.append(i+1);
    }

    return w;
}
