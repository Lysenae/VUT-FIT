// Subor: vectorops.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Operacie s vektormi

#include "vectorops.h"

/// Privatny konstruktor.
VecOps::VecOps()
{
}

/// Privatny destruktor.
VecOps::~VecOps()
{
}

/// Prevedie vektor integerov na retazec spojeny ,.
///
/// @param vec Vektor integerov.
/// @param noSpace Bez medzier medzi prvkami?.
/// @return Retazec.
std::string VecOps::toString(std::vector<int> vec, bool noSpace)
{
    std::string s = "";

    for(unsigned int i=0; i<vec.size(); i++)
    {
        s += std::to_string(vec[i]);
        if(i < vec.size()-1)
        {
            if(noSpace)
            {
                s += ",";
            }
            else
            {
                s += ", ";
            }
        }
    }

    return s;
}

/// Zoradi prvky vektoru.
///
/// @param vec Odkaz na vektor integerov
void VecOps::sort(std::vector<int> &vec)
{
    std::sort(vec.begin(), vec.end());
}

/// Zoradi prvky vektoru.
///
/// @param vec Odkaz na vektor retazcov
void VecOps::sort(std::vector<std::string> &vec)
{
    std::sort(vec.begin(), vec.end());
}

/// Obrati poradie prvkov vektoru.
///
/// @param vec Odkaz na vektor integerov
void VecOps::reverse(std::vector<int> &vec)
{
    std::reverse(vec.begin(), vec.end());
}

/// Obrati poradie prvkov vektoru.
///
/// @param vec Odkaz na vektor retazcov
void VecOps::reverse(std::vector<std::string> &vec)
{
    std::reverse(vec.begin(), vec.end());
}

/// Vytvori vektor integerov z retazca oddelenim prvkov podla delimiter.
///
/// Vrati prazdny vektor ak narazi na chybu.
/// @param str Retazec.
/// @param delimiter Oddelovac.
/// @return Vketor integerov.
std::vector<int> VecOps::fromStringI(std::string str, const char delimiter)
{
    std::vector<int> vec;
    std::string s = "";

    for(unsigned int i=0; i<str.length(); i++)
    {
        if((str[i] >= '0' && str[i] <= '9') || (str[i] == '-'))
        {
            s += str[i];
            if(i == str.length()-1)
            {
                vec.push_back(StrOps::toInt(s));
            }
        }
        else if(str[i] == delimiter)
        {
            vec.push_back(StrOps::toInt(s));
            s = "";
        }
        else if((delimiter != ' ') && (str[i] == ' '))
        {
            if(i == str.length()-1)
            {
                vec.push_back(StrOps::toInt(s));
            }
            continue;
        }
        else // Chybny znak, ktory nemozno previest na cislo
        {
            vec.clear();
            break;
        }
    }
    return vec;
}
