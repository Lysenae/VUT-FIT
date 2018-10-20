// Subor: random.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Metody pre pracu s RNG.

#include "random.h"

/// Privatny konstruktor.
Random::Random()
{
}

/// Privatny destruktor.
Random::~Random()
{
}

/// Vygeneruje nahodne cislo v rozsahu low-high, vratane.
///
/// @param low Najnizsie cislo
/// @param high Najvyssie cislo.
/// @return Cislo z intervalu <low;hgh>.
unsigned int Random::random(unsigned int low, unsigned int high)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(low, high);

    return dist(mt);
}

/// Nahodne porozhadzuje prvky vektoru.
///
/// @param vec Odkaz na vektor integerov.
void Random::shuffle(std::vector<int> &vec)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(vec.begin(), vec.end(), mt);

}
