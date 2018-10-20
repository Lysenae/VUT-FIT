// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef HASH_H
#define HASH_H

#include <map>

#include "types.h"

///
/// \brief Asociativne pole
///
class Hash
{
public:
    Hash() {}
    void add_existing(std::string key, std::string value);
    StrVect keys();
    bool has_key(std::string key);
    bool has_value(std::string key, std::string value);
    void add_value(std::string key, std::string value);
    StrVect values(std::string key);
    void print();

private:
    std::map<std::string, StrVect*> m_map;
};

#endif // HASH_H
