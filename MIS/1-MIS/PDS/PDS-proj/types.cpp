// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "types.h"

///
/// \brief Hexadecimalna hodnota 16b unsigned cisla
/// \param t
/// \return string
///
std::string str_bytes16(uint16_t t)
{
    uchar u[2];
    char buffer[5];
    memcpy(u, &t, S_USHORT);
    sprintf(buffer, "%02X %02X", u[0], u[1]);
    return std::string(buffer);
}

///
/// \brief Uchar zo stringu
/// \param s
/// \return uchar
///
uchar str_to_uch(std::string s)
{
    size_t ptr;
    int val = std::stoi(s, &ptr);
    if(val >= (int)B_UCHAR_MIN && val <= (int)B_UCHAR_MAX && ptr == s.size())
        return (uchar) val;
    std::cerr << "str_to_uch(" << s << "): Conversion error" << std::endl;
    return (uchar) 0;
}

///
/// \brief Hexadecimalna 8b unsigned cisla
/// \param t
/// \return string
///
std::string str_bytes8(uchar t)
{
    char buffer[2];
    sprintf(buffer, "%02X", t);
    return std::string(buffer);
}

///
/// \brief Uchar zo stringu
/// \param s
/// \return uchar
///
uchar literal_to_uchr(std::string s)
{
    if(s.size() <= 2)
    {
        long r = strtol(s.c_str(), nullptr, 16);
        return (uchar) r;
    }
    return 0x00;
}

///
/// \brief Rozdeli retazec
/// \param s
/// \param delimiter
/// \return StrVect
///
StrVect split_str(std::string s, char delimiter)
{
    StrVect v;
    std::string t = "";
    for(uint i=0; i<s.size(); ++i)
    {
        if(s[i] == delimiter)
        {
            v.push_back(t);
            t = "";
        }
        else
        {
            t += s[i];
            if(i == s.size()-1)
            {
                v.push_back(t);
            }
        }
    }
    return v;
}
