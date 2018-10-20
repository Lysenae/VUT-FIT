// Subor: stringops.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Operacie s retazcami

#include "stringops.h"

/// Privatny konstruktor.
StrOps::StrOps()
{
}

/// Privateny destruktor.
StrOps::~StrOps()
{
}

/// Rozdeli retazec na podretazce na zaklade rozdelovaca.
///
/// @param s Retazec
/// @param delimiter Rozdelovac
/// @return Zoznam retazcov, na ktore bol rozdeleny povodny.
std::vector<std::string> StrOps::split(std::string s, const char delimiter)
{
    std::vector<std::string> vec;
    std::string str = "";

    for(unsigned int i=0; i<s.length(); i++)
    {
        if(s[i] == delimiter)
        {
            vec.push_back(str);
            str = "";
        }
        else
        {
            str += s[i];

            if(i == s.length()-1)
            {
                vec.push_back(str);
            }
        }
    }

    return vec;
}

/// Prevedie retazec na integer.
///
/// @param s Retazec.
/// @return Integer.
int StrOps::toInt(std::string s)
{
    return std::stoi(s);
}

/// Prevedie integer na retazec.
///
/// @param i Inetger.
/// @return Retazec.
std::string StrOps::iToStr(int i)
{
    return std::to_string(i);
}
