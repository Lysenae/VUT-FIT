// Subor: objectives.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Triede reprezentujuca mnozinu hracov.

#include "players.h"

/// Konstruktor.
///
/// Vytvori mnozinu hracov a urci im startovne pozicie.
/// @param count Pocet hracov.
/// @param size Velkost plochy
Players::Players(int count, int size)
{
    if(count != 2 && count != 4)
    {
        throw std::invalid_argument("Player count must be 2 or 4");
    }

    _count   = count;
    _current = 1;

    int sxs = size*size;
    _positions.push_back(-1);       // Neplatna hodnota, aby sedeli indexy hracov
    _positions.push_back(sxs-size); // Hrac 1 zacina v lavom dolnom rohu
    _positions.push_back(size-1);   // Hrac 2 zacina v pravom hornom rohu
    if(count == 4)
    {
        _positions.push_back(0);        // Hrac 3 zacina v lavom hornom rohu
        _positions.push_back(sxs-1);    // Hrac 4 zacina v pravom dolnom rohu
    }

    // Na poziciu 0 sa vlozi 0, aby index odpovedal cislu hraca
    // vector.size() == count + 1
    for(int i=0; i<=_count; i++)
    {
        _scores.push_back(0);
        _objectives.push_back(0);
    }
}

/// Konstruktor.
///
/// Vytvori mnozinu hracov na zaklade zadanych udajov.
/// @param count Pocet hracov.
/// @param current Hrac na tahu.
/// @param positions Zoznam pozicii hracov.
/// @param scores Skore hracov.
/// @param objectives Ulohy hracov.
Players::Players(int count, int current, std::vector<int> positions,
std::vector<int> scores, std::vector<int> objectives)
{
    _count      = count;
    _current    = current;
    _positions  = positions;
    _scores     = scores;
    _objectives = objectives;
}

/// Destruktor.
Players::~Players()
{
}

/// Dalsi hrac.
void Players::next()
{
    if(_current == _count)
    {
        _current = 1;
    }
    else
    {
        _current += 1;
    }
}

/// Predchadzajuc hrac.
void Players::previous()
{
    if(_current == 1)
    {
        _current = _count;
    }
    else
    {
        _current -= 1;
    }
}

/// Pocet hracov.
///
/// @return Pocet hracov.
int Players::count() const
{
    return _count;
}

/// Pozicia aktualneho hraca.
///
/// @return Poziciu aktualneho hraca.
int Players::currentPosition() const
{
    return _positions[current()];
}

/// Nastavi poziciu hraca.
///
/// @param player Cislo hraca.
/// @param position Pozicia.
void Players::updatePosition(int player, int position)
{
    _positions[player] = position;
}

/// Cislo aktualneho hraca.
///
/// @return Cislo aktualneho hraca.
int Players::current() const
{
    return _current;
}

/// Skore aktualneho hraca.
///
/// @return Skore aktualneho hraca.
int Players::currentScore() const
{
    return _scores[current()];
}

/// Zvysi skore hraca.
///
/// @param player Cislo hraca.
void Players::incScore(int player)
{
    _scores[player] = _scores[player] + 1;
}

/// Znizi skore hraca.
///
/// @param player Cislo hraca.
void Players::decScore(int player)
{
    if(_scores[player] > 0)
    {
        _scores[player] = _scores[player] - 1;
    }
}

/// Vrati skore hraca.
///
/// @return Skore hraca player.
int Players::score(int player) const
{
    return _scores[player];
}

/// Vrati cislo ulohy aktualneho hraca.
///
/// @return Cislo ulohy.
int Players::currentObjective() const
{
    return _objectives[current()];
}

/// Nastavi cislo ulohy hraca.
///
/// @param player Cislo hraca.
/// @param objective Cislo ulohy.
void Players::updateObjective(int player, int objective)
{
    _objectives[player] = objective;
}

/// Zoznam pozicii hracov.
///
/// @return Zoznam pozicii hracov.
std::vector<int> Players::positionList() const
{
    return _positions;
}

/// Zoznam bodov hracov.
///
/// @return Zoznam bodov hracov.
std::vector<int> Players::scoreList() const
{
    return _scores;
}

/// Zoznam uloh hracov.
///
/// @return Zoznam uloh hracov.
std::vector<int> Players::objectiveList() const
{
    return _objectives;
}

/// Meno hraca.
///
/// @param player Cislo hraca.
/// @return Meno hraca player.
std::string Players::toString(int player)
{
    std::string s;
    switch(player)
    {
        case 1:
            s = "Blue";
            break;
        case 2:
            s = "Red";
            break;
        case 3:
            s = "Green";
            break;
        case 4:
            s = "Yellow";
            break;
        default:
            s = "Invalid player ID " + std::to_string(player);
            break;
    }

    return s;
}
