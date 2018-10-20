// Subor: tilemask.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Trieda masky dlazdic

#include "tilemask.h"

/// Konstruktor.
TileMask::TileMask()
{
    _items.clear();
}

/// Konstruktor.
///
/// @param size Velkost.
/// @param t_tiles Pocet T dlazdic.
/// @param l_tiles Pocet L dlazdic.
/// @param i_tiles Pocet I dlazdic.
/// @param players Pocet hracov.
TileMask::TileMask(int size, int &t_tiles, int &l_tiles, int &i_tiles, int players)
{
    int sqr_size = size * size;
    std::vector<int> emptyVec;

    for(int i=0; i<=sqr_size; i++)
    {
        _items.push_back(TileMaskItem(Undef, 1, -1, emptyVec, 0));
    }

    int row;
    int col;

    // Vypln rohove a neparne*neparne dlazdice
    for(int i=0; i<sqr_size; i++)
    {
        // i % size (mod) => index stlpca
        // i / size (div) => index riadku

        _items[i].setPosition(i);

        row = i / size;
        col = i % size;

        //Rohove dlazdice
        if(i == 0 || i == size-1 || i == size*(size-1) || i == size*size-1)
        {
            _items[i].setType(L_Tile);
            l_tiles--;
            if(i == 0)
            {
                // Lavy horny roh
                _items[i].setRotation(2);
                if(players == 4)
                {
                    _items[i].addPlayer(3);
                }
            }
            else if(i == size-1)
            {
                // Pravy horny roh
                _items[i].setRotation(3);
                if(players == 2 || players == 4)
                {
                    _items[i].addPlayer(2);
                }
            }
            else if(i == size*(size-1))
            {
                // Lavy dolny roh
                _items[i].setRotation(1);
                if(players == 2 || players == 4)
                {
                    _items[i].addPlayer(1);
                }
            }
            else if(i == size*size-1)
            {
                // Pravy dolny roh
                _items[i].setRotation(4);
                if(players == 4)
                {
                    _items[i].addPlayer(4);
                }
            }
        }
        else if(even(row) && even(col))
        {
            _items[i].setType(T_Tile);
            t_tiles--;
            if(row == 0)
            {
                // Horny kraj
                _items[i].setRotation(3);
            }
            else if(row == size-1)
            {
                // Dolny kraj
                _items[i].setRotation(1);
            }
            else if(col == 0)
            {
                // Lavy kraj
                _items[i].setRotation(2);
            }
            else if(col == size-1)
            {
                // Pravy kraj
                _items[i].setRotation(4);
            }
            else
            {
                // Neokrajove, niekde v strede
                _items[i].setRotation(Random::random(1, 4));
            }
        }
    }

    unsigned int avl;
    // Vypln zvysne dlazdice
    for(int i=0; i<sqr_size; i++)
    {
        if(_items[i].type() == Undef)
        {
            avl = avaliableTypes(t_tiles, l_tiles, i_tiles);
            _items[i].setType(getTileType(Random::random(1, avl), avl, t_tiles, l_tiles, i_tiles));
        }
    }

    _items.back().setType(t_tiles > 0 ? T_Tile : l_tiles > 0 ? L_Tile : I_Tile);
    _items.back().setRotation(1);
    _items.back().setPosition(-1);
}

/// Konstruktor.
///
/// @param ts Mnozina pointrov na dlazdice.
/// @param ft Pointer na volnu dlazdicu
TileMask::TileMask(TileSet *ts, Tile* ft)
{
    Tile *temp;
    for(int i=0; i<ts->size(); i++)
    {
        temp = ts->at(i);
        _items.push_back(TileMaskItem(temp->tileType(), temp->rotation(), temp->position(),
            temp->playerList(), temp->loot()));
    }

    if(ft != NULL)
    {
        _items.push_back(TileMaskItem(ft->tileType(), ft->rotation(), ft->position(),
            ft->playerList(), ft->loot()));
    }
}

/// Destruktor.
TileMask::~TileMask()
{
}

/// Neprane cislo?
///
/// @return Bool.
bool TileMask::even(int i) const
{
    if(i % 2 == 0)
    {
        return true;
    }

    return false;
}

/// Vrati pocet dostupnych typov dlazdic.
///
/// @param t Int.
/// @param l Int.
/// @param i Int.
/// @return Integer.
unsigned int TileMask::avaliableTypes(int t, int l, int i) const
{
    unsigned int n = 0;
    if(t > 0) n++;
    if(l > 0) n++;
    if(i > 0) n++;

    return n;
}

/// Urci typ na zaklade dostupnosti typov.
///
/// @param idx Index.
/// @param n Pocet dostupnych.
/// @param t T dlazdice.
/// @param l L dlazdice.
/// @param i I dlazdice.
/// @return Typ dlazdice.
TileType TileMask::getTileType(int idx, unsigned int n, int &t, int &l, int &i)
{
    if(n == 3)
    {
        if(idx == 1)
        {
            t--;
            return T_Tile;
        }
        else if(idx == 2)
        {
            l--;
            return L_Tile;
        }
        else
        {
            i--;
            return I_Tile;
        }
    }
    else if(n == 2)
    {
        if(idx == 1)
        {
            if(t > 0)
            {
                t--;
                return T_Tile;
            }
            else
            {
                i--;
                return I_Tile;
            }
        }
        else
        {
            if(l > 0)
            {
                l--;
                return L_Tile;
            }
            else
            {
                i--;
                return I_Tile;
            }
        }
    }
    else
    {
        if(t > 0)
        {
            t--;
            return T_Tile;
        }
        else if(l > 0)
        {
            l--;
            return L_Tile;
        }
        else
        {
            i--;
            return I_Tile;
        }
    }
}

/// Pristupovy operator.
///
/// @return Prvok na indexe i.
TileMaskItem& TileMask::operator[](unsigned int i)
{
    return _items[i];
}

/// Pristupovy operator.
///
/// @return Prvok na poslednom indexe.
TileMaskItem& TileMask::last()
{
    return _items.back();
}

/// Velkost.
///
/// @return Velkost.
int TileMask::size() const
{
    return _items.size();
}

/// Skopiruj obsah z i prvku do j prvku.
///
/// @param i Index i.
/// @param j Index j.
void TileMask::copy(unsigned int i, unsigned int j)
{
    if((i < _items.size()) && (j < _items.size()))
    {
        _items[j].setRotation(_items[i].rotation());
        _items[j].setType(_items[i].type());
        _items[j].setPlayers(_items[i].players());
        _items[j].setObjective(_items[i].objective());
    }
}

/// Nastavi prvok i podla dat.
/// @see Tile::Tile
void TileMask::set(unsigned int i, TileType type, unsigned int rot, int pos,
std::vector<int> players, int objective)
{
    if(i < _items.size())
    {
        _items[i].setPosition(pos);
        _items[i].setRotation(rot);
        _items[i].setType(type);
        _items[i].setPlayers(players);
        _items[i].setObjective(objective);
    }
}

/// Prida novy prvok na zaklade dat.
/// @see Tile::Tile()
void TileMask::addItem(TileType type, unsigned int rot, int pos,
std::vector<int> players, int objective)
{
    _items.push_back(TileMaskItem(type, rot, pos, players, objective));
}

/// Najde indexy prvkov, ktore neobsahuju ulohu.
///
/// @param size Velkost.
/// @return Zoznam volnych prvkov.
std::vector<int> TileMask::findAvailable(int size) const
{
    std::vector<int> avl;
    int pos;
    int end = (int)_items.size() == size*size ? _items.size() : _items.size()-1;

    for(int i=0; i<end; i++)
    {
        pos = _items[i].position();
        if((pos != 0) && (pos != size-1) && (pos != size*size-size) &&
        (pos != size*size-1) && (_items[i].objective() == 0))
        {
            avl.push_back(i);
        }
    }

    return avl;
}

/// Retazcova podoba prvku na indexe index.
///
/// @param index Index.
/// @return Retazec.
std::string TileMask::itemToString(unsigned int index) const
{
    std::string s = "";

    if(index < _items.size())
    {
        s += std::to_string((int)_items[index].type()) + ";";
        s += std::to_string(_items[index].position()) + ";";
        s += std::to_string(_items[index].rotation()) + ";";
        s += VecOps::toString(_items[index].players(), true) + ";";
        s += std::to_string(_items[index].objective());
    }

    return s;
}

/// Je prazdna?
///
/// @return Bool.
bool TileMask::empty() const
{
    return _items.empty();
}

//################################################################################
//################################# TileMaskItem #################################
//################################################################################

/// Konstruktor.
/// @see Tile::Tile()
TileMaskItem::TileMaskItem(TileType type, unsigned int rotation, int position,
std::vector<int> players, int objective)
{
    _type = type;
    _rotation  = rotation;
    _position  = position;
    _players   = players;
    _objective = objective;
}

/// Destruktor.
TileMaskItem::~TileMaskItem()
{
}

/// Getter typu.
/// @see Tile::type()
TileType TileMaskItem::type() const
{
    return _type;
}

/// Setter typu.
/// @see Tile::setType()
void TileMaskItem::setType(TileType type)
{
    _type = type;
}

/// Getter rotacie.
/// @see Tile::rotation()
unsigned int TileMaskItem::rotation() const
{
    return _rotation;
}

/// Setter typu.
/// @see Tile::setRotation()
void TileMaskItem::setRotation(unsigned int rotation)
{
    _rotation = rotation;
}

/// Getter pozicie.
/// @see Tile::position()
int TileMaskItem::position() const
{
    return _position;
}

/// Setter typu.
/// @see Tile::setPosition()
void TileMaskItem::setPosition(int position)
{
    _position = position;
}

/// Getter zoznamu hracov.
/// @see Tile::playerList()
std::vector<int> TileMaskItem::players() const
{
    return _players;
}

/// Setter zoznamu hracov.
///
/// @param players Zoznam hracov.
void TileMaskItem::setPlayers(std::vector<int> players)
{
    _players = players;
}

/// Prida hraca.
///
/// @param player Cislo hraca.
void TileMaskItem::addPlayer(int player)
{
    _players.push_back(player);
}

/// Getter ulohy.
/// @see Tile::loot()
int TileMaskItem::objective() const
{
    return _objective;
}

/// Setter ulohy.
/// @see Tile::setLoot()
void TileMaskItem::setObjective(int objective)
{
    _objective = objective;
}
