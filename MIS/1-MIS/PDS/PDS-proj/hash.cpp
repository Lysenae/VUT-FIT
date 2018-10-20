// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "hash.h"

///
/// \brief Prida hodnotu do existujuceho kluca
/// \param key
/// \param value
///
void Hash::add_existing(std::string key, std::string value)
{
    if(has_key(key))
    {
        if(!has_value(key, value))
            add_value(key, value);
    }
}

///
/// \brief Zoznam klucov
/// \return
///
StrVect Hash::keys()
{
    StrVect v;
    for(std::map<std::string, StrVect*>::const_iterator it = m_map.begin();
    it != m_map.end(); ++it)
      v.push_back(it->first);
    return v;
}

///
/// \brief Zisti ci hash obsahuje zadany kluc
/// \param key
/// \return bool
///
bool Hash::has_key(std::string key)
{
    StrVect ks = keys();
    for(std::string k : ks)
    {
        if(k == key)
            return true;
    }
    return false;
}

///
/// \brief  Zisti ci hash obsahuje zadanu hodnotu v zadanom kluci
/// \param key
/// \param value
/// \return bool
///
bool Hash::has_value(std::string key, std::string value)
{
    if(!has_key(key))
        return false;
    StrVect *vals = m_map[key];
    for(std::string v : *vals)
    {
        if(v == value)
            return true;
    }
    return false;
}

///
/// \brief Prida neduplicitnu hodnotu do zadaneho kluca
/// \param key
/// \param value
///
void Hash::add_value(std::string key, std::string value)
{
    if(!has_key(key))
    {
        m_map[key] = new StrVect();
    }

    if(!has_value(key, value))
        m_map[key]->push_back(value);
}

///
/// \brief Vrati vektor hodnot pod danym klucom alebo prazdny vektor
/// \param key
/// \return StrVect
///
StrVect Hash::values(std::string key)
{
    StrVect v;
    if(has_key(key))
        v = *(m_map[key]);
    return v;
}

///
/// \brief Vypise obsah
///
void Hash::print()
{
    StrVect ks = keys();
    for(std::string k : ks)
    {
        std::cout << k << std::endl;
        StrVect *vals = m_map[k];
        for(std::string v : *vals)
            std::cout << "\t" << v << std::endl;
    }
}
