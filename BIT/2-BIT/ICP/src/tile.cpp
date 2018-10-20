// Subor: stringops.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda dlazdice

#include "tile.h"

/// Konstruktor.
///
/// @param type Typ dlazdice -T, L, I
/// @param position Pozicia v hracej ploche.
/// @param rotation Natocenie.
/// @param palyer Zoznam hracov na dlazdici.
/// @param objective Cislo ulohy na dlazdici.
Tile::Tile(TileType type, int position, unsigned int rotation, std::vector<int> players, int objective)
{
    _type        = type;
    _rotation    = new TileRotation(rotation);
    _loot        = objective;
    _active      = false;
    _rotatable   = false;
    _position    = position;
    _players     = players;

    setPaths();
}

/// Destruktor.
Tile::~Tile()
{
}

/// Getter pre rotaciu.
///
/// @return Rotacia.
unsigned int Tile::rotation() const
{
    return _rotation->val();
}

/// Getter pre typ.
///
/// @return Typ.
TileType Tile::tileType() const
{
    return _type;
}

/// Getter pre poziciu.
///
/// @return Pozicia.
int Tile::position() const
{
    return _position;
}

/// Setter pre rotaciu.
///
/// @param position Pozicia.
void Tile::setPosition(int position)
{
    _position = position;
}

/// Getter pre ulohu.
///
/// @return Cislo ulohy.
int Tile::loot() const
{
    return _loot;
}

/// Setter pre rotaciu.
///
/// @param loot Uloha.
void Tile::setLoot(int loot)
{
    // Mozeme len pridat alebo odobrat, nie nahradit
    if(((_loot == 0) && (loot > 0)) || ((_loot > 0) && (loot == 0)))
    {
        _loot = loot;
    }
}

/// Vrati true ak na dlazdici je uloha.
///
/// @return Bool.
bool Tile::hasLoot() const
{
    return _loot > 0 ? true : false;
}

/// Zoznam hracov na dlazdici.
///
/// @return Zoznam hracov na dlazdici.
std::vector<int> Tile::playerList() const
{
    return _players;
}

/// Zoznam hracov na dlazdici.
///
/// @return Odkaz na zoznam hracov na dlazdici.
std::vector<int>& Tile::playerListRef()
{
    return _players;
}

/// Prida hraca na dlazdicu.
///
/// @param player Cislo hraca.
void Tile::addPlayer(int player)
{
    _players.push_back(player);
    VecOps::sort(_players);
}

/// Odstrani hraca z dlazdice.
///
/// @param player Cislo hraca.
void Tile::removePlayer(int player)
{
    std::vector<int> tempVec;
    int tempVal;

    while(!_players.empty())
    {
        tempVal = _players.back();
        _players.pop_back();
        if(tempVal != player)
        {
            tempVec.push_back(tempVal);
        }
    }

    VecOps::sort(tempVec);
    _players = tempVec;
}


/// Je aktivna?
///
/// @return Bool.
bool Tile::active() const
{
    return _active;
}

/// Nastavi aktivitu.
///
/// @param active Bool.
void Tile::setActive(bool active)
{
    _active = active;
}

/// Otacatelna?
///
/// @return Bool.
bool Tile::rotatable() const
{
    return _rotatable;
}

/// Nastavi otacatelnost.
///
/// @param active Bool.
void Tile::setRotatable(bool rotatable)
{
    _rotatable = rotatable;
}

/// Cesta nahor?
///
/// @return Bool.
bool Tile::pointsUp()
{
    return _pathTop;
}

/// Cesta vpravo?
///
/// @return Bool.
bool Tile::pointsRight()
{
    return _pathRight;
}

/// Cesta nadol?
///
/// @return Bool.
bool Tile::pointsDown()
{
    return _pathBottom;
}

/// Cesta vlavo?
///
/// @return Bool.
bool Tile::pointsLeft()
{
    return _pathLeft;
}

/// Textova podoba dlazdice.
///
/// Vytvori textovu podobu dlazdice a vrati ju ako zoznam retazcov.
/// @param loot Cislo ulohy.
/// @return Zoznam retazcov, dlzka ma byt 5.
std::vector<std::string> Tile::toString(int loot) const
{
    std::vector<std::string> strings;
    strings.push_back("┌───────┐");
    strings.push_back("│" + lootToString(loot) + "   " + posToString() +"│");
    strings.push_back("│ " + activeToString() + " " + symbolByRotation() + "   │");
    strings.push_back("│" + playersToString() + "│");
    strings.push_back("└───────┘");

    return strings;
}

/// Vypise textovu podobu dlazdice na stdout.
///
/// @param loot Cislo ulohy.
void Tile::print(int pLoot) const
{
    std::vector<std::string> strs = toString(pLoot);
    for(unsigned int i=0; i<strs.size(); i++)
    {
        std::cout << strs[i] << std::endl;
    }
}

/// Vypise napovedu pre rotacie dlazdice.
void Tile::rotationHint() const
{
    std::cout << "1  2  3  4" << std::endl;
    if(tileType() == T_Tile)
    {
        std::cout << "╩  ╠  ╦  ╣" << std::endl;
    }
    else if(tileType() == L_Tile)
    {
        std::cout << "╚  ╔  ╗  ╝" << std::endl;
    }
    else if(tileType() == I_Tile)
    {
            std::cout << "║  ═  ║  ═" << std::endl;
    }
    else
    {
        std::cout << std::endl;
    }
}

/// Vrati symbol dlazdice podla rotacie ako retazec o dlzke 1.
///
/// @return std::string.
std::string Tile::symbolByRotation() const
{
    std::string c; // c.length = 1
    switch(tileType())
    {
        case T_Tile:
        {
            switch(rotation())
            {
                case 1:
                    c = "╩";
                    break;
                case 2:
                    c = "╠";
                    break;
                case 3:
                    c = "╦";
                    break;
                case 4:
                    c = "╣";
                    break;
                default:
                    c = " ";
                    break;
            }
            break;
        }
        case L_Tile:
        {
            switch(rotation())
            {
                case 1:
                    c = "╚";
                    break;
                case 2:
                    c = "╔";
                    break;
                case 3:
                    c = "╗";
                    break;
                case 4:
                    c = "╝";
                    break;
                default:
                    c = " ";
                    break;
            }
            break;
        }
        case I_Tile:
        {
            switch(rotation())
            {
                case 1:
                    c = "║";
                    break;
                case 3:
                    c = "║";
                    break;
                case 2:
                    c = "═";
                    break;
                case 4:
                    c = "═";
                    break;
                default:
                    c = " ";
                    break;
            }
            break;
        }
        default:
            c = " ";
            break;
    }

    return c;
}

/// Vrati retazec symbolizujuci ulohu na dlazdici.
///
/// @return std::string.
std::string Tile::lootToString(int pLoot) const
{
    int lootId = loot();
    std::string s = " "; // s.length = 1

    if(pLoot == lootId)
    {
        s = Objectives::toString(pLoot);
    }

    return s;
}

/// Vrati retazec s hracmi na dlazdici o dlzke 7.
///
/// @return std::string.
std::string Tile::playersToString() const
{
    std::vector<int> pl = playerList();
    std::string s = ""; // s.length = 7

    for(unsigned int i=0; i<pl.size(); i++)
    {
        if(pl[i] == 1)
        {
            s += "B";
        }
        else if(pl[i] == 2)
        {
            s += "R";
        }
        else if(pl[i] == 3)
        {
            s += "G";
        }
        else if(pl[i] == 4)
        {
            s += "Y";
        }
    }

    // Doplnenie medzier
    int diff = Tile::TILE_WIDTH - s.length();
    std::string spaces = "";
    for(int i=0; i<diff; i++)
    {
        spaces += " ";
    }
    s += spaces;

    return s;
}

/// Vrati retazec symbolizujuci, ci je dlazdica aktivna.
///
/// @return std::string.
std::string Tile::activeToString() const
{
    std::string s; // s.length = 1
    s = active() ? "A" : " ";

    return s;
}

/// Vrati retazec s cislom indexu o dlzke 3.
///
/// @return std::string.
std::string Tile::posToString() const
{
    std::string s; // s.length = 3
    s = std::to_string(position());

    if(s.length() == 1)
    {
        s = "  " + s;
    }
    else if(s.length() == 2)
    {
        s = " " + s;
    }

    return s;
}

/// Nastavi cesty.
void Tile::setPaths()
{
    switch(_type)
    {
        case T_Tile:
        {
            switch(_rotation->val())
            {
                case 1:
                    _pathTop    = true;
                    _pathRight  = true;
                    _pathBottom = false;
                    _pathLeft   = true;
                    break;
                case 2:
                    _pathTop    = true;
                    _pathRight  = true;
                    _pathBottom = true;
                    _pathLeft   = false;
                    break;
                case 3:
                    _pathTop    = false;
                    _pathRight  = true;
                    _pathBottom = true;
                    _pathLeft   = true;
                    break;
                case 4:
                    _pathTop    = true;
                    _pathRight  = false;
                    _pathBottom = true;
                    _pathLeft   = true;
                    break;
            }
            break;
        }
        case L_Tile:
        {
            switch(_rotation->val())
            {
                case 1:
                    _pathTop    = true;
                    _pathRight  = true;
                    _pathBottom = false;
                    _pathLeft   = false;
                    break;
                case 2:
                    _pathTop    = false;
                    _pathRight  = true;
                    _pathBottom = true;
                    _pathLeft   = false;
                    break;
                case 3:
                    _pathTop    = false;
                    _pathRight  = false;
                    _pathBottom = true;
                    _pathLeft   = true;
                    break;
                case 4:
                    _pathTop    = true;
                    _pathRight  = false;
                    _pathBottom = false;
                    _pathLeft   = true;
                    break;
            }
            break;
        }
        case I_Tile:
        {
            switch(_rotation->val())
            {
                case 1:
                    _pathTop    = true;
                    _pathRight  = false;
                    _pathBottom = true;
                    _pathLeft   = false;
                    break;
                case 2:
                    _pathTop    = false;
                    _pathRight  = true;
                    _pathBottom = false;
                    _pathLeft   = true;
                    break;
                case 3:
                    _pathTop    = true;
                    _pathRight  = false;
                    _pathBottom = true;
                    _pathLeft   = false;
                    break;
                case 4:
                    _pathTop    = false;
                    _pathRight  = true;
                    _pathBottom = false;
                    _pathLeft   = true;
                    break;
            }
            break;
        }
        default:
        {
            _pathTop    = false;
            _pathRight  = false;
            _pathBottom = false;
            _pathLeft   = false;
            break;
        }
    }
}

/// Nastavi nasledujucu rotaciu.
void Tile::rotationNext()
{
    _rotation->next();
}

/// Rotacia na stupen val.
///
/// @param val Cislo pozadovanej rotacie.
void Tile::rotate(unsigned int val)
{
    while(rotation() != val)
    {
        rotationNext();
    }

    setPaths();
}
