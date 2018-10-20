// Subor: objectives.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Triede reprezentujuca mnozinu uloh.

#include "objectives.h"

/// Konstruktor.
///
/// Vytvori novu mnozinu uloh o velkosti 12 alebo 24 uloh.
/// @param size Mnozstvo uloh
Objectives::Objectives(int size)
{
    if(size != 12 && size != 24)
    {
        throw std::invalid_argument("Deck size must be 12 or 24");
    }

    std::vector<int> v;
    for(int i=1; i<=24; i++)
    {
        v.push_back(i);
    }

    Random::shuffle(v);
    if(size == 12)
    {
        std::vector<int> tmp;
        for(int i=0; i<12; i++)
        {
            tmp.push_back(v[i]);
        }
        _objectives = tmp;
    }
    else
    {
        _objectives = v;
    }
}

/// Konstruktor.
///
/// Vytvori mnozinu uloh z predaneho zoznamu.
/// @param objectives Zoznam uloh.
Objectives::Objectives(std::vector<int> objectives)
{
    _objectives = objectives;
}

/// Destruktor.
Objectives::~Objectives()
{
}

/// Pocet zostavajucich uloh.
///
/// Vrati velkost zoznamu uloh.
/// @return Velkost zoznamu uloh.
int Objectives::remaining() const
{
    return _objectives.size();
}

/// Cislo nasledujucej ulohy.
///
/// Vrati cislo nasledujucej ulohy a vyhodi ju zo zoznamu.
/// @return Cislo nasledujucej ulohy.
int Objectives::next()
{
    if(_objectives.size() > 0)
    {
        int n = _objectives.back();
        _objectives.pop_back();
        return n;
    }

    return 0;
}

/// Prida ulohu do zoznamu.
///
/// Prida ulohu na koniec zoznamu.
/// @param objective Cislo ulohy.
void Objectives::add(int objective)
{
    VecOps::reverse(_objectives);
    _objectives.push_back(objective);
    VecOps::reverse(_objectives);
}

/// Vymena ulohy.
///
/// Vrati ulohu zo spodu zoznamu a na vrchol vlozi predavanu.
/// @param Predavana uloha.
/// @return Vratena uloha.
int Objectives::giveBack(int objective)
{
    _objectives.push_back(objective);
    VecOps::reverse(_objectives);

    int o = _objectives.back();
    _objectives.pop_back();
    VecOps::reverse(_objectives);

    return o;
}

/// Zoznam uloh.
///
/// Vrati zoznam uloh.
/// @return Zoznam uloh.
std::vector<int> Objectives::objectiveList() const
{
    return _objectives;
}

/// Znak reprezentujuci ulohu.
///
/// Vrati znak reprezentujuci ulohu.
/// @return Znak reprezentujuci ulohu ako retazec o dlzke 1.
std::string Objectives::toString(int objective)
{
    std::string s = "";
    std::string symbols = " 1234567890!@#$%&*=(){}[]";

    if(objective > 0 && objective <= 24)
    {
        s += symbols[objective];
    }
    else
    {
        s = "";
    }

    return s;
}
