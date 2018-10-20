// Subor: boardmodel.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: BoardModel predstavuje hraciu dosku labyrintu.
//        Je to zakladna triedy programu, ktora riadi celu logiku a
//        vyuziva ostatne moduly.

#include "boardmodel.h"

using namespace std;

/// Konstruktor.
///
/// Konstruktor inicializuje triedu a inicializuje premenne
BoardModel::BoardModel()
{
    _size        = 0;  // Velkost plochy
    _l_tiles     = 0;  // pocet L dlazdic
    _t_tiles     = 0;  // pocet T dlazdic
    _i_tiles     = 0;  // pocet I dlazdic
    _phase       = 0;  // faza (posun dlazdic alebo posun figurky)
    _forbidden   = -1; // index dlazdice ktoru nemozno posuvat

    _tiles = new TileSet();
    _free_tile   = NULL;
    _players     = NULL;
    _objectives  = NULL;

    _rSteps.clear();
}

/// Destruktor.
BoardModel::~BoardModel()
{
}

/// Vytvori novu hru.
///
/// Vytvori novu hru pomocou nacitania potrebnych udajov zo standardneho vstupu.
void BoardModel::createGame()
{
    int s, o, p;
    while(true) // Nacitanie velkosti plochy
    {
        cout << "Size (5, 7, 9, 11): ";
        cin >> s;
        if(cin.fail())
        {
            cin.clear();
        }
        cin.ignore();

        if((s == 5) || (s == 7) || (s == 9) || (s == 11))
        {
            break;
        }
    }

    while(true) // Nacitanie poctu hracov
    {
        cout << "Players (2, 4): ";
        cin >> p;
        if(cin.fail())
        {
            cin.clear();
        }
        cin.ignore();

        if((p == 2) || (p == 4))
        {
            break;
        }
    }

    while(true) // Nacitanie poctu predmetov
    {
        cout << "Objectives (12, 24): ";
        cin >> o;
        if(cin.fail())
        {
            cin.clear();
        }
        cin.ignore();

        if((o == 12) || (o == 24))
        {
            break;
        }
    }

    Players *pls    = new Players(p, s);
    Objectives *obj = new Objectives(o);
    setPlayers(pls);
    setBoardSize(s);
    setObjectives(obj);
    setTilesCount();
    setGoal(objectives()->remaining()/players()->count());

    int t_tiles, l_tiles, i_tiles;
    tileCounters(t_tiles, l_tiles, i_tiles);
    TileMask tmask(boardSize(), t_tiles, l_tiles, i_tiles, players()->count());

    initializeObjectives(tmask);

    run(tmask);
}

/// Spusti hru, bud novu alebo nacitanu
///
/// Na zaklade masky dlazdic tmask vykresli hraciu plochu a dalej potom
/// prijima prikazy zo standardneho vstupu. Hra bezi do vitazstva jedneho
/// z hracov alebo pouzitim Ctrl+C.
/// @param tmask Maska dlazdic, na zaklade ktorej sa vytvori hracia plocha
int BoardModel::run(TileMask tmask)
{
    int input;
    std::string strIn;
    std::vector<int> avl;
    TileMask tm = tmask;

    while(true)
    {
        cout << "###################################################################" << endl;
        showBoard(tm);
        printScores();
        cout << Players::toString(players()->current()) + " player's turn" << endl;
        cout << "You are searching for: " << Objectives::toString(players()->currentObjective()) << endl;

        if(phase() == 0) // Faza posunu dlazdice
        {
            freeTile()->print(players()->currentObjective());
            freeTile()->rotationHint();

            while(true) // Otocenie volnej dlazdice
            {
                cout << "Rotate free tile (1-4): ";
                cin >> input;
                if(cin.fail())
                {
                    cin.clear();
                }
                cin.ignore();

                if((input >= 1) && (input <= 4))
                {
                    freeTile()->rotate(input);
                    freeTile()->print(players()->currentObjective());
                    break;
                }
            }

            cout << endl;

            while(true) // Posun dlazdic
            {
                cout << "Shift from: ";
                cin >> input;
                if(cin.fail())
                {
                    cin.clear();
                }
                cin.ignore();

                if((input < boardSize()*boardSize()) && (input >= 0))
                {
                    if(tileSet()->at(input)->active())
                    {
                        break;
                    }
                }
            }

            // Pred posunom sa ulozi aktualny stav pre pripadne navratenie pomocou operacie Undo
            ShiftType st;
            reverseSteps().push_back(ReverseStep(phase(), forbidden(), calculateOpposite(input, st), -1, -1));
            tm = shiftTiles(input);
            showBoard(tm);
            switchPhase();
        }
        else if(phase() == 1) // Faza posunu hraca
        {
            avl = findPath(players()->currentPosition());
            activateTiles(avl);
            VecOps::sort(avl);
            string s = VecOps::toString(avl);

            while(true) // Posun figurky
            {
                cout << "Move to (" << s << "): ";
                cin >> input;
                if(cin.fail())
                {
                    cin.clear();
                }
                cin.ignore();

                if(std::find(avl.begin(), avl.end(), input) != avl.end())
                {
                    break;
                }
            }

            int tileLoot = tileSet()->at(input)->loot();
            reverseSteps().push_back(ReverseStep(phase(), -1, -1, players()->currentPosition(), tileLoot));
            moveTo(input);

            if(players()->currentScore() == goal())
            {
                cout << Players::toString(players()->current()) << " player has won the game!";
                break;
            }

            tm = TileMask(tileSet(), freeTile());
            showBoard(tm);

            printScores();

            switchPhase();
            players()->next();
        }

        while(true) // Ulozit hru, vratit kroky alebo pokracovat
        {
            cout << "(S)ave, (U)ndo or (C)ontinue?: ";
            cin >> strIn;
            if(cin.fail())
            {
                cin.clear();
                break;
            }
            cin.ignore();

            if((strIn == "S") || (strIn == "s")) // Ulozit
            {
                cout << "File name: ";
                cin >> strIn;
                save(strIn);
                break;
            }
            else if((strIn == "U") || (strIn == "u")) // Undo
            {
                if(reverseSteps().size() > 0)
                {
                    tm = applyReverseStep();

                    if(!tm.empty())
                    {
                        showBoard(tm);
                        activateTiles(phase()==0 ? findShiftable() : findPath(players()->currentPosition()));
                    }
                }
                else
                {
                    cerr << "No reverse steps" << endl;
                }
            }
            else
            {
                break;
            }
        }
    }

    return 0;
}

// Gettery a settery

/// Pocet bodov potrebnych na vitazstvo.
///
/// Getter pre pocet bodov potrebnych na vitazstvo.
/// @return Pocet bodov na vitazstvo.
int BoardModel::goal() const
{
    return _goal;
}

/// Nastavi pocet bodov potrebnych na vitazstvo.
///
/// Setter pre pocet bodov potrebnych na vitazstvo.
/// @param goal Pocet bodov.
void BoardModel::setGoal(int goal)
{
    _goal = goal;
}

/// Velkost hracej plochy, jednej strany.
///
/// Getter velkost hracej plochy.
/// @return Velkost hracej plochy.
int BoardModel::boardSize() const
{
    return _size;
}

/// Nastavi velkost hracej plochy, jednej strany.
///
/// Setter velkost hracej plochy.
/// @param size Velkost hracej plochy.
void BoardModel::setBoardSize(int size)
{
    _size = size;
}

/// Objekt hracov.
///
/// Getter objekt hracov.
/// @return Ukazovatel na objekt hracov
Players* BoardModel::players() const
{
    return _players;
}

/// Nastavi ukazovatel na objekt hracov.
///
/// Setter pre objekt hracov.
/// @param players Ukazovael na objekt typu Players.
void BoardModel::setPlayers(Players *players)
{
    _players = players;
}

/// Zrusi objekt hracov.
///
/// Korektne uvolni pamat vyhradenu pre hracov.
void BoardModel::freePlayers()
{
    if(_players != NULL)
    {
        delete _players;
        _players = NULL;
    }
}

/// Objekt uloh.
///
/// Getter na ukazovatel na objekt uloh.
/// @return Ukazovatel na objekt uloh.
Objectives* BoardModel::objectives() const
{
    return _objectives;
}
/// Nastavi ukazovatel na objekt uloh.
///
/// Setter pre objekt uloh.
/// @param objectives Ukazovatel na objekt typu Objectives.
void BoardModel::setObjectives(Objectives *objectives)
{
    _objectives = objectives;
}

/// Zrusi objekt uloh.
///
/// Korektne uvolni pamat vyhradenu pre ulohy.
void BoardModel::freeObjectives()
{
    if(_objectives != NULL)
    {
        delete _objectives;
        _objectives = NULL;
    }
}

/// Ziska pocitadla typov dlazdic
///
/// Nastavi pocitadla predane odkazom poctom dlazdic daneho typu.
/// @param t_tiles Pocitadlo pre dlazdice tvaru T.
/// @param l_tiles Pocitadlo pre dlazdice tvaru L.
/// @param i_tiles Pocitadlo pre dlazdice tvaru I.
void BoardModel::tileCounters(int &t_tiles, int &l_tiles, int &i_tiles) const
{
    t_tiles = _t_tiles;
    l_tiles = _l_tiles;
    i_tiles = _i_tiles;
}

/// Nastavi pocty dlazdic kazdeho typu.
///
/// Na zaklade velkosti hracej plochy inicializuje pocitadla dlazdic.
/// Celkovo rozdeluje size*size+1 dlazdic, kde size je velkost hracej plochy.
void BoardModel::setTilesCount()
{
    int total = 0;
    _l_tiles  = 0;
    _t_tiles  = 0;
    _i_tiles  = 0;

    int n = boardSize() * boardSize() + 1;
    while(total <= n)
    {
        _t_tiles++;
        total++;

        if(total == n)
            break;

        _l_tiles++;
        total++;

        if(total == n)
            break;

        _i_tiles++;
        total++;
    }
}

/// Aktualna faza.
///
/// Vrati aktualnu fazu hry, 0 znaci fazu posunu dlazdic, 1 znaci
/// fazu posunu figurky aktualneho hraca.
/// @return Aktualnu fazu, 0 alebo 1.
int BoardModel::phase() const
{
    return _phase;
}

/// Zmeni fazu hry
///
/// Zmeni hodnotu fazy hry, z 0 na 1 alebo z 1 na 0.
void BoardModel::switchPhase()
{
    _phase = _phase == 0 ? 1 : 0;
}

/// Nastavi fazu hry.
///
/// Nastavi fazu hry, pouziva sa len pri nacitani ulozenych hier,
/// inac moze sposobit podivne stavy. Pri zadani cisla ineho ako 0 alebo 1
/// sa stav sam upravi po zavolani switchPhase().
/// @param phase Faza hry.
void BoardModel::setPhase(int phase)
{
    _phase = phase;
}

/// Index dlazdice, ktorou nemozno zahajit posun dlazdic.
///
/// Vrati index dlazdice, ktorou nemozno zahajit posun dlazdic,
/// nastavuje sa v metode shiftTiles.
/// @preturn
int BoardModel::forbidden() const
{
    return _forbidden;
}

/// Nastavi index dlazdice, ktorou nemozno zahajit posun dlazdic.
///
/// Setter pre index dlazdice, ktorou nemozno zahajit posun dlazdic.
/// @param forbidden Index dlazdice.
void BoardModel::setForbidden(int forbidden)
{
    _forbidden = forbidden;
}

/// Mnozina dlazdic, z ktorej je zlozena hracia plocha.
///
/// Mnozina dlazdic, z ktorej je zlozena hracia plocha. Neobsahuje
/// volnu dlazdicu.
/// @return Ukazovatel na instanciu triedy TileSet.
TileSet* BoardModel::tileSet()
{
    return _tiles;
}

/// Volna dlazdica.
///
/// Volna dlazdica, t.j., ktoru mozno otacat a vkladat do rhacej plochy.
/// @return Ukazovatel na objekt volnej dlazdice.
Tile* BoardModel::freeTile()
{
    return _free_tile;
}

/// Nastavi volnu dalzdicu.
///
/// Setter pre ukazovatel na volnu dalzdicu.
/// @param tile Ukazovatel na instanciu triedy Tile.
void BoardModel::setFreeTile(Tile *tile)
{
    _free_tile = tile;
}

/// Fronta reverznych krokov
///
/// Vracia odkaz na frontu reverznych krokov, ktoru vyuziva operacia Undo.
/// @return Odkaz na vektor instancii triedy ReverseStep.
std::vector<ReverseStep>& BoardModel::reverseSteps()
{
    return _rSteps;
}

// Ine metody

/// Inicializuje pociatocne ulohy hracov.
///
/// Kazdemu hracovi prideli ulohu a urci jej polohu na hracej ploche.
/// @param tm Maska dlazdic.
void BoardModel::initializeObjectives(TileMask &tm)
{
    std::vector<int> available;
    available = tm.findAvailable(boardSize());
    Random::shuffle(available);

    for(int i=1; i<=players()->count(); i++)
    {
        int obj = objectives()->next();
        players()->updateObjective(i, obj);
        tm[available.back()].setObjective(obj);
        available.pop_back();
    }
}

/// Zoznam posuvatelnych dlazdic.
///
/// Ziska zoznam dlazdic, ktore mozu zahajit posun stlpca alebo riadku hracej
/// plochy, dlazdicu urcenu pomocou setForbidden() vynechava.
/// @return Zoznam indexov dlazdic, ktore mozu zahajit posun.
std::vector<int> BoardModel::findShiftable()
{
    vector<int> sh;
    int size = boardSize();

    for(int i=0; i<size*size; i++)
    {
        if(i != forbidden())
        {
            if((i % 2 == 1) && ((i / size == 0) || (i % size == 0) ||
            (i % size == size-1) || (i / size == size-1)))
            {
                sh.push_back(i);
            }
        }
    }

    return sh;
}

/// Vypocita index dlazdice na opacnom konci plochy.
///
/// Vypocita index dlazdice na opacnom konci plochy a urci smer posunu.
/// @param pos Index dlazdice, pre ktoru sa hlada protilahla.
/// @param st Odkaz na premennu typu ShiftType, do ktorej sa ulozi indikator smeru posunu.
/// @return Index protilahlej dlazdice
int BoardModel::calculateOpposite(int pos, ShiftType &st)
{
    int opposite;
    int size = boardSize();

    // Horny okraj
    if(pos / size == 0)
    {
        opposite = pos + size* (size-1);
        st = Shift_Down;
    }
    // Lavy okraj
    else if(pos % size == 0)
    {
        opposite = pos + size - 1;
        st = Shift_Right;
    }
    // Pravy okraj
    else if(pos % size == size-1)
    {
        opposite = pos - (size - 1);
        st = Shift_Left;
    }
    // Dolny okraj
    else if(pos / size == size-1)
    {
        opposite = pos - size * (size-1);
        st = Shift_Up;
    }
    // Nemalo by nastat
    else
    {
        opposite = -1;
        st = Shift_None;
    }

    return opposite;
}

/// Zahaji posun dlazdic.
///
/// Zahaji posun dlazdic, nastavi dlazdicu, ktoru nemozno posuvat v najblizsom tahu a
/// vytvori masku dlazdic na zaklade, ktroej sa plocha znovu vykresli.
/// @return Masku dlazdic.
TileMask BoardModel::shiftTiles(int pos)
{
    ShiftType st;
    int opposite = calculateOpposite(pos, st);
    setForbidden(opposite);
    TileMask tm(tileSet(), _free_tile);

    int div = pos / boardSize();
    int mod = pos % boardSize();
    int max = boardSize()-1;
    int min = 0;

    // Zabrani memory leaku pripouziti na dlazdicu, ktora nie je na kraji
    // hracej plochy
    if(div == min || div == max || mod == min || mod == max)
    {
        // Nastavi velkost rozdielu dvoch indexov v zavislosti od smeru posunu
        // Vypocitava sa od najvzdialenejsieho (protilahleho) indexu, size = velkost plochy
        // Posun nadol   = -size
        // Posun nahor   = size
        // Posun doprava = -1
        // Posun dolava  = 1
        int dec;
        if(st == Shift_Down)
        {
            dec = -boardSize();
        }
        else if(st == Shift_Up)
        {
            dec = boardSize();
        }
        else if(st == Shift_Right)
        {
            dec = -1;
        }
        else if(st == Shift_Left)
        {
            dec = 1;
        }
        else // Nemalo by nastat
        {
            dec = 0;
        }

        // Docasne uloz protilahlu dlazdicu a potom ju uloz do free tile
        // Volna dlazdica (free tile) je posledny prvok TileMask
        int              npos      = opposite;
        unsigned int     temp_rot  = tm[opposite].rotation();
        TileType         temp_type = tm[opposite].type();
        std::vector<int> temp_pls  = tm[opposite].players();
        int              temp_obj  = tm[opposite].objective();

        // Posun
        for(int i=0; i<(_size-1); i++)
        {
            tm.copy(npos+dec, npos);
            npos += dec;
        }

        // Nastav volnu dlazdicu
        std::vector<int> emptyVec;
        tm.set(pos, tm.last().type(), tm.last().rotation(), pos, temp_pls,
            tm.last().objective());
        tm.set(tm.size()-1, temp_type, temp_rot, -1, emptyVec, temp_obj);

        for(int i=0; i<tm.size(); i++)
        {
            if(!tm[i].players().empty())
            {
                for(unsigned int j=0; j<tm[i].players().size(); j++)
                {
                    players()->updatePosition(tm[i].players()[j], tm[i].position());
                }
            }
        }
    }

    return tm;
}

/// Najde dlazdice, na ktore su spojene so zadanou dlazdicou.
///
/// Najde dlazdice na zaklade testu dostupnych smerov danej a susednej dlazdice. Vytvara
/// zoznam open, v ktorom potom test opakuje pre kazdu najdenu dlazdicu. Test sa ukonci
/// az dojde na koniec zoznamu open.
/// @param pos Index dlazdice, z ktoreho sa zacina hladat cesta.
/// @return Zoznam indexov dlazdic, na ktore sa da dostat zo zadanej dlazdice.
std::vector<int> BoardModel::findPath(int pos)
{
    vector<int> open;
    open.push_back(pos);
    int size = boardSize();
    int idx;

    unsigned int i=0;
    while(i < open.size())
    {
        // Dlazdica pod
        idx = open[i]+size;
        if((idx < size*size) && (tileSet()->at(open[i])->pointsDown()) && (tileSet()->at(idx)->pointsUp()))
        {
            if(find(open.begin(), open.end(), idx) == open.end())
            {
                open.push_back(idx);
            }
        }

        // Dlazdica nad
        idx = open[i]-size;
        if((idx >= 0) && (tileSet()->at(open[i])->pointsUp()) && (tileSet()->at(idx)->pointsDown()))
        {
            if(find(open.begin(), open.end(), idx) == open.end())
            {
                open.push_back(idx);
            }
        }

        // Dlazdica nalavo
        idx = open[i]-1;
        if((idx >= 0) && (tileSet()->at(open[i])->pointsLeft()) && (tileSet()->at(idx)->pointsRight()))
        {
            if(find(open.begin(), open.end(), idx) == open.end())
            {
                open.push_back(idx);
            }
        }

        // Dlazdica napravo
        idx = open[i]+1;
        if((idx < size*size) && (tileSet()->at(open[i])->pointsRight()) && (tileSet()->at(idx)->pointsLeft()))
        {
            if(find(open.begin(), open.end(), idx) == open.end())
            {
                open.push_back(idx);
            }
        }

        i++;
    }

    return open;
}

/// Posun hraca
///
/// Presunie hraca na zadanu dlazdicu a pripadne vezme predmet.
/// @param pos Index cielovej dlazdice.
/// @param canPickup Povolenie brania predmetov.
void BoardModel::moveTo(int pos, bool canPickUp)
{
    int player = players()->current();
    int oldPos = players()->currentPosition();

    Tile *temp = tileSet()->at(pos);
    int tileLoot = temp->loot();

    if(canPickUp && (tileLoot == players()->currentObjective()))
    {
        objectives()->add(temp->loot());
        players()->incScore(players()->current());
        temp->setLoot(0);

        int obj = objectives()->next();
        players()->updateObjective(players()->current(), obj);
        spawnObjective(obj);
    }

    // Nic nerob, ked sa neposuva na inu dlazdicu
    if(oldPos != pos)
    {
        tileSet()->at(oldPos)->removePlayer(player);
        temp->addPlayer(player);
        players()->updatePosition(player, pos);
    }
}

/// Aktivuje dlazdice.
///
/// Aktivuje dlazdice dane zoznamom.
/// @param tileIdxs Zoznam indexov dlazdic na aktivovanie
void BoardModel::activateTiles(std::vector<int> tileIdxs)
{
    TileSet *tiles = tileSet();
    for(int i=0; i<tiles->size(); i++)
    {
        tiles->at(i)->setActive(false);
    }

    for(unsigned int i=0; i<tileIdxs.size(); i++)
    {
        tiles->at(tileIdxs[i])->setActive(true);
    }
}

/// Vygeneruje ulohu a umiestni ju na plochu
///
/// Vygeneruje ulohu a umiestni ju na nahodnu alebo zadanu volnu dlazdicu.
/// @param objective Cislo ulohy
/// @param position Volitelny parameter, vynucuje umiestnenie ulohy na danu dlazdicu.
void BoardModel::spawnObjective(int objective, int position)
{
    int pos;
    if(position <= -1)
    {
        TileMask tm(tileSet());
        std::vector<int> avl = tm.findAvailable(boardSize());
        Random::shuffle(avl);
        pos = avl.back();
    }
    else
    {
        pos = position;
    }

    tileSet()->at(pos)->setLoot(objective);
}

/// Odstrani ulohu z hracej plochy.
///
/// Odstrani zadanu ulohu zo vsetkych dlazdic.
/// @param objective Cislo ulohy
void BoardModel::removeObjective(int objective)
{
    TileSet *ts = tileSet();
    for(int i=0; i<ts->size(); i++)
    {
        if(ts->at(i)->loot() == objective)
        {
            ts->at(i)->setLoot(0);
        }
    }

    if(freeTile()->loot() == objective)
    {
        freeTile()->setLoot(0);
    }
}

/// Aplikuje operaciu Undo.
///
/// Aplikuje reverzny krok na konci zoznamu reverznych krokov. Nastavi potrebne
/// premenne a prida/odstrani (ne)potrebne objekty. Vytvori novu masku dlazdic.
/// @return Masku dlazdic
TileMask BoardModel::applyReverseStep()
{
    TileMask tm;

    if(!reverseSteps().empty())
    {
        ReverseStep top = reverseSteps().back();
        switchPhase();

        // Vratenie posunu dlazdice
        if(top.type == 0)
        {
            tm = shiftTiles(top.shiftPos);
            setForbidden(top.shiftForbidden);
        }

        // Vratenie posunu hraca
        if(top.type == 1)
        {
            players()->previous();

            if(top.pickedLoot > 0)
            {
                removeObjective(players()->currentObjective());
                int o = objectives()->giveBack(players()->currentObjective());
                players()->updateObjective(players()->current(), o);
                spawnObjective(o, players()->currentPosition());
                players()->decScore(players()->current());
            }

            moveTo(top.movePos, false);
            tm = TileMask(tileSet(), freeTile());
        }

        // Vyhod krok zo zoznamu
        reverseSteps().pop_back();
    }

    return tm;
}

/// Vypise aktualne skore hracov.
void BoardModel::printScores() const
{
    for(int i=1; i<=players()->count(); i++)
    {
        std::cout << Players::toString(i) << ":\t" << players()->score(i) << std::endl;
    }
}

// Virtualne metody

/// Vykresli hraciu plochu.
///
/// Na zaklade masky dlazdic vykresli hraciu plochu.
/// @param tm Maska dlazdic.
void BoardModel::showBoard(TileMask &tm)
{
    // Vymaze a znovu inicializuje vektor TileSet podla predanej masky
    if(tileSet()->size() > 0)
    {
        for(int i=0; i<tileSet()->size(); i++)
        {
            delete tileSet()->at(i);
        }

        tileSet()->empty();
    }

    for(int i=0; i<boardSize()*boardSize(); i++)
    {
        Tile *tile = new Tile(tm[i].type(), tm[i].position(), tm[i].rotation(),
            tm[i].players(), tm[i].objective());
        tileSet()->append(tile);
    }

    if(_free_tile != NULL)
    {
        delete _free_tile;
        _free_tile = NULL;
    }

    Tile *ft = new Tile(tm.last().type(), tm.last().position(), tm.last().rotation(),
        tm.last().players(), tm.last().objective());
    setFreeTile(ft);
    freeTile()->setRotatable(true);
    freeTile()->setActive(true);

    activateTiles(phase() == 0 ? findShiftable() : findPath(players()->currentPosition()));


    vector< vector<string> > vs;
    string s = "";

    // Uloz do vs vsetky dlazdice v textovej podobe
    for(int i=0; i<tileSet()->size(); i++)
    {
        vs.push_back(tileSet()->at(i)->toString(players()->currentObjective()));
    }

    // Vypis (vykresli) dlazdice po riadkoch
    for(int k=0; k<boardSize(); k++)
    {
        for(int j=0; j<Tile::STR_SIZE; j++)
        {
            for(int i=0; i<boardSize(); i++)
            {
                s += vs[k*boardSize()+i][j];
            }
            cout << s << endl;
            s = "";
        }
    }
}

/// Ulozenie hry
///
/// Ulozi hru do suboru fName, ulozene hry su vo formate csv.
/// @param fName Nazov suboru, do ktoreho sa ulozi hra.
void BoardModel::save(std::string fName)
{
    ofstream file;
    file.open(fName, ios_base::out);

    if(file.fail())
    {
        cerr << "Could not open file to save!" << endl;
        file.clear( );
    }
    else
    {
        string s = "";

        s = "Size;" + to_string(boardSize());
        file << s << "\n";
        s = "Objectives;" + VecOps::toString(objectives()->objectiveList(), true);
        file << s << "\n";
        s = "Players;" +
            StrOps::iToStr(players()->count()) + ";" +
            StrOps::iToStr(players()->current()) + ";" +
            VecOps::toString(players()->positionList(), true) + ";" +
            VecOps::toString(players()->scoreList(), true) + ";" +
            VecOps::toString(players()->objectiveList(), true);
        file << s << "\n";
        s = "Forbidden;" + StrOps::iToStr(forbidden());
        file << s << "\n";
        s = "Phase;" + StrOps::iToStr(phase());
        file << s << "\n";
        s = "Goal;" + StrOps::iToStr(goal());
        file << s << "\n";

        TileMask tm(tileSet(), freeTile());
        for(int i=0; i<tm.size(); i++)
        {
            s = "Tile;" + tm.itemToString(i);
            file << s << "\n";
        }

        file.close();
        cout << "Save complete!" << endl;
    }
}

/// Nacitanie hry
///
/// Nacita hru zo suboru fName a skontroluje validitu dat.
/// @param fName Cesta k suboru s ulozenou hrou.
void BoardModel::load(std::string fName)
{
    ifstream file;
    file.open(fName, ios_base::in);
    if(file.fail())
    {
        cerr << "Could not open file to load!" << endl;
        file.clear();
    }
    else
    {
        string s = "";
        vector<string> ss;
        int i = 0;

        // Size
        int size = -1;
        // Objectives
        vector<int> objectives;
        // Players
        int pCount = -1;
        int pCurrent = -1;
        vector<int> pPositions;
        vector<int> pScores;
        vector<int> pObjectives;
        // Forbidden
        int forbidden = -2;
        // Phase
        int phase = -1;
        // Goal
        int goal = -1;
        // Tile
        TileType tType = Undef;
        int tPosition = -2;
        unsigned int tRotation = -1;
        vector<int> tPlayers;
        int tObjective = 0;

        TileMask tm;

        while(getline(file, s))
        {
            ss = StrOps::split(s, ';');

            if((ss[0] == "Size") && (ss.size() == 2))
            {
                size = StrOps::toInt(ss[1]);
            }
            else if((ss[0] == "Objectives") && (ss.size() == 2))
            {
                objectives = VecOps::fromStringI(ss[1]);
            }
            else if((ss[0] == "Players") && (ss.size() == 6))
            {
                cout << s << endl;
                pCount = StrOps::toInt(ss[1]);
                pCurrent = StrOps::toInt(ss[2]);
                pPositions = VecOps::fromStringI(ss[3]);
                pScores = VecOps::fromStringI(ss[4]);
                pObjectives = VecOps::fromStringI(ss[5]);
            }
            else if((ss[0] == "Forbidden") && (ss.size() == 2))
            {
                forbidden = StrOps::toInt(ss[1]);
            }
            else if((ss[0] == "Phase") && (ss.size() == 2))
            {
                phase = StrOps::toInt(ss[1]);
            }
            else if((ss[0] == "Goal") && (ss.size() == 2))
            {
                goal = StrOps::toInt(ss[1]);
            }
            else if((ss[0] == "Tile") && (ss.size() == 6))
            {
                tType = (TileType)StrOps::toInt(ss[1]);
                tPosition = StrOps::toInt(ss[2]);
                tRotation = (unsigned int)StrOps::toInt(ss[3]);
                tPlayers = VecOps::fromStringI(ss[4]);
                tObjective = StrOps::toInt(ss[5]);

                tm.addItem(tType, tRotation, tPosition, tPlayers, tObjective);
            }
            else
            {
                break;  // Chybny riadok
            }

            ss.clear();
            i++;
        }

        if(i != 6 + size*size + 1)
        {
            cerr << "Load failed, file may be corrupted!" << endl;
        }
        else
        {
            setBoardSize(size);
            setForbidden(forbidden);
            setPhase(phase);
            setGoal(goal);
            freePlayers();
            freeObjectives();

            Players *p = new Players(pCount, pCurrent, pPositions, pScores, pObjectives);
            setPlayers(p);

            Objectives *o = new Objectives(objectives);
            setObjectives(o);

            cout << "Load complete!" << endl;

            run(tm);
        }
    }
}
