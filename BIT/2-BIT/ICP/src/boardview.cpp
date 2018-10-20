// Subor: boardview.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: BoardView GUI nadstavba triedy BoardModel
//        UI bolo vygenerovane automaticky (boardview.ui)

#include "boardview.h"
#include "ui_boardview.h"

/// Konstruktor.
BoardView::BoardView(QString fPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoardView)
{
    ui->setupUi(this);

    _geometry    = sizeHint();
    _newGameDlg  = NULL;
    _rows        = NULL;
    _mainLayout  = NULL;
    _ftInfo      = NULL;
    _objInfo     = NULL;
    _pLoot       = NULL;

    QObject::connect(ui->actionNewGame, SIGNAL(triggered()), this, SLOT(newGame()));
    QObject::connect(ui->actionSaveGame, SIGNAL(triggered()), this, SLOT(handleSave()));
    QObject::connect(ui->actionLoadGame, SIGNAL(triggered()), this, SLOT(handleLoad()));
    QObject::connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(handleUndo()));

    QRect r = this->geometry();
    r.moveCenter(QApplication::desktop()->availableGeometry().center());
    this->setGeometry(r);

    if(fPath != "")
    {
        load(fPath.toStdString());
    }
}

/// Destruktor.
BoardView::~BoardView()
{
    delete ui;
}

/// Zahajenie tvorby novej hry.
///
/// Slot vyvolany stlacenim tlacidla menu New Game. Vykresli dialog NewGameDlg.
void BoardView::newGame()
{
    if(!_newGameDlg)
    {
       _newGameDlg =  new NewGameDialog(this);
       QObject::connect(_newGameDlg, SIGNAL(passParams(int, int, int)), this, SLOT(createGame(int, int, int)));
    }

    _newGameDlg->show();
    _newGameDlg->raise();
    _newGameDlg->activateWindow();
}

/// Vytvori novu hru.
///
/// Na zaklade dat z NewGameDlg vytvori novu hru.
/// @param s Velkost plochy.
/// @param p Pocet hracov.
/// @param o Pocet uloh.
void BoardView::createGame(int s, int p, int o)
{
    if(s == 0 || p == 0 || o == 0)
    {
        QMessageBox errBox;
        errBox.critical(0, "Error","Failed to process New Game settings!");
        errBox.setFixedSize(500, 200);
        exit(1);
    }

    ui->actionSaveGame->setEnabled(true);

    Players *pls    = new Players(p, s);
    Objectives *obj = new Objectives(o);

    clearBoard();
    setBoardSize(s);
    setPlayers(pls);
    setObjectives(obj);
    setTilesCount();
    setGoal(objectives()->remaining()/players()->count());

    // Maska pre vytvorenie hracej plochy
    int t_tiles, l_tiles, i_tiles;
    tileCounters(t_tiles, l_tiles, i_tiles);
    TileMask tmask(boardSize(), t_tiles, l_tiles, i_tiles, players()->count());

    initializeObjectives(tmask);

    showBoard(tmask);
}

/// Vykresli hraciu plochu.
///
/// Na zaklade masky dlazdic vykresli hraciu plochu.
/// @param tm Maska dlazdic.
void BoardView::showBoard(TileMask tm)
{
    if(tm.size() != boardSize()*boardSize()+1)
    {
        QMessageBox errBox;
        errBox.critical(0, "Error","Invalid Tile Mask size!");
        errBox.setFixedSize(500, 200);
        exit(1);
    }

    // Volna dlazdica
    TileView *ft = new TileView(tm.last().type(), tm.last().position(), tm.last().rotation(),
        tm.last().players(), phase(), tm.last().objective());
    QObject::connect(this, SIGNAL(phaseChanged(int)), ft, SLOT(onPhaseChanged(int)));
    setFreeTile(ft);
    freeTile()->setRotatable(true);
    freeTile()->setActive(true);

    // Layout
    _mainLayout = new QHBoxLayout();
    _rows       = new QVBoxLayout();
    _infoLayout = new QVBoxLayout();

    // Informacne vypisy
    QString temp = QString(Players::toString(players()->current()).c_str()) + "'s turn";
    _infoLabels.append(new QLabel(temp));

    for(int i=0; i<players()->count(); i++)
    {
        temp = QString(Players::toString(i+1).c_str());
        temp += ":\t" + QString::number(players()->score(i+1));
        _infoLabels.append(new QLabel(temp));
    }

    // Informacne labely
    _ftInfo  = new QLabel("Free Tile");
    _objInfo = new QLabel("Objective");
    _pLoot   = new QLabel("");
    _pLoot->setPixmap(QPixmap(TileView::imageByLootId(players()->currentObjective())));

    // Layout pre informacnu cast
    _infoLayout->setContentsMargins(0, 0, 15, 0);
    _infoLayout->setSpacing(0);
    _infoLayout->addWidget(_ftInfo, 0, Qt::AlignHCenter);
    _infoLayout->addWidget(dynamic_cast<TileView*>(freeTile()), 0, Qt::AlignHCenter);
    _infoLayout->addStretch();
    for(int i=0; i<_infoLabels.size(); i++)
    {
        _infoLayout->addWidget(_infoLabels[i], 0, Qt::AlignTop);
    }
    _infoLayout->addStretch();
    _infoLayout->addWidget(_objInfo, 0, Qt::AlignHCenter);
    _infoLayout->addWidget(_pLoot, 0, Qt::AlignHCenter);

    // Hlavny layout
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(0);
    _mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    _mainLayout->addLayout(_infoLayout);
    _mainLayout->addLayout(_rows);

    _w = new QWidget();
    _w->setLayout(_mainLayout);
    ui->scrollArea->setWidget(_w);

    for(int i=0; i<boardSize(); i++)
    {
        QHBoxLayout *col = new QHBoxLayout();
        _cols.push_back(col);
        _rows->addLayout(col);
    }

    // Inicializacia dlazdic
    int idx;
    for(int i=0; i<boardSize()*boardSize(); i++)
    {
        idx = i / boardSize(); // Index pozicie dlazdice
        TileView *tile = new TileView(tm[i].type(), tm[i].position(), tm[i].rotation(),
            tm[i].players(), phase(), tm[i].objective());
        tileSet()->append(tile);
        _cols[idx]->addWidget(tile, 0, Qt::AlignTop);
        QObject::connect(tile, SIGNAL(shiftInitiated(int)), this, SLOT(handleShiftTiles(int)));
        QObject::connect(tile, SIGNAL(moveInitiatedTo(int)), this, SLOT(handleMoveTo(int)));
        QObject::connect(tile, SIGNAL(pathFindingInitiated(int)), this, SLOT(handlePathFinding(int)));
        QObject::connect(this, SIGNAL(phaseChanged(int)), tile, SLOT(onPhaseChanged(int)));
    }

    activateTiles(phase() == 0 ? findShiftable() : findPath(players()->currentPosition()));
    QApplication::processEvents();
    resize(minimumSizeHint());
    updateGeometry();
    ui->scrollArea->updateGeometry();
    ui->scrollArea->setAlignment(Qt::AlignCenter);
    update();
    setLootVisibility(players()->currentObjective());
}

/// Zrusi hraciu plochu.
///
/// Zrusi vsetky widgety hracej plochy.
void BoardView::clearBoard()
{
    // Zrus dlazdice
    tileSet()->empty();

    if(!_cols.isEmpty())
    {
        for(int i=0; i<_cols.size(); i++)
        {
            if(_cols[i]->layout() != NULL )
            {
                QLayoutItem* item;
                while((item = _cols[i]->layout()->takeAt(0)) != NULL)
                {
                    delete item->widget();
                    delete item;
                }
                delete _cols[i]->layout();
            }
        }
    }

    // Zrus labely
    if(_ftInfo != NULL)
    {
        delete _ftInfo;
        _ftInfo = NULL;
    }

    if(_objInfo != NULL)
    {
        delete _objInfo;
        _objInfo = NULL;
    }

    if(_pLoot != NULL)
    {
        delete _pLoot;
        _pLoot = NULL;
    }

    while(!_infoLabels.empty())
    {
        delete _infoLabels.back();
        _infoLabels.pop_back();
    }

    // Zrus layouty
    if(ui->scrollArea->layout() != NULL )
    {
        QLayoutItem* item;
        while((item = ui->scrollArea->widget()->layout()->takeAt(0)) != NULL)
        {
            delete item->widget();
            delete item;
        }
        delete ui->scrollArea->widget()->layout();
        delete ui->scrollArea->widget();
    }

    while(!_cols.isEmpty())
    {
        _cols.pop_front();
    }

    delete freeTile();

    resize(_geometry);
    update();
}

/// Zahajenie posunu dlazdic.
///
/// Slot aktovovany kliknutim na posuvatelnu dlazdicu.
/// @param pos Index dlazdice.
void BoardView::handleShiftTiles(int pos)
{
    ShiftType st;
    reverseSteps().push_back(ReverseStep(phase(), forbidden(), calculateOpposite(pos, st), -1, -1));
    switchPhase();
    TileMask tm = shiftTiles(pos);
    clearBoard();
    showBoard(tm);
    setLootVisibility(players()->currentObjective());
}

/// Zahajenie posunu figurky hraca.
///
/// Slot aktivovany kliknutim na dostupnu dlazdicu.
/// @param pos Index dlazdice.
void BoardView::handleMoveTo(int pos)
{
    int tileLoot = tileSet()->at(pos)->loot();
    reverseSteps().push_back(ReverseStep(phase(), -1, -1, players()->currentPosition(), tileLoot));
    moveTo(pos);
    updateScores();

    // Koniec?
    if(players()->currentScore() != goal())
    {
        players()->next();
        setLootVisibility(players()->currentObjective());
        QString temp = QString(Players::toString(players()->current()).c_str()) + "'s turn";
        _infoLabels[0]->setText(temp);
        _pLoot->setPixmap(QPixmap(TileView::imageByLootId(players()->currentObjective())));
        switchPhase();
        activateTiles(findShiftable());
    }
    else
    {
        QMessageBox infoBox;
        QString str = QString(Players::toString(players()->current()).c_str()) + " player has won the game!";
        infoBox.information(0, "Victory", str);
        infoBox.setFixedSize(500, 200);
        ui->actionSaveGame->setEnabled(false);
        std::vector<int> emptyVec;
        activateTiles(emptyVec);
    }
}

/// Zahaji vyhladavanie dostupnych dlazdic.
///
/// Slot aktivovany po ukonceni fazy posunu dlazdic.
/// @param pos Index dlazdice, od ktorej sa vyhldava cesta.
void BoardView::handlePathFinding(int pos)
{
    activateTiles(findPath(pos));
}

/// Zahaji ukladanie hry.
///
/// Zobrazi dialog a zavola metodu save.
void BoardView::handleSave()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
        QString(), tr("ICP Files (*.icp);;All Files (*)"));

    if(!fn.isEmpty())
    {
        save(fn.toStdString());
    }
}

/// Zahaji operaciu Undo.
///
/// Skusi zavolat operaciu Undo a pripadne prekresli plochu.
void BoardView::handleUndo()
{
    TileMask tm;
    if(reverseSteps().size() > 0)
    {
        int type = reverseSteps().back().type;
        tm = applyReverseStep();

        if(!tm.empty())
        {
            if(type == 0)
            {
                clearBoard();
                showBoard(tm);
            }

            activateTiles(phase()==0 ? findShiftable() : findPath(players()->currentPosition()));
            setLootVisibility(players()->currentObjective());
            _pLoot->setPixmap(QPixmap(TileView::imageByLootId(players()->currentObjective())));
            updateScores();
        }
    }
}

/// Zahaji nacitavanie.
///
/// Vykresli dialog a pripadne nacita hru.
void BoardView::handleLoad()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
        QString(), tr("ICP Files (*.icp);;All Files (*)"));

    if(!fn.isEmpty())
    {
        load(fn.toStdString());
    }

    ui->actionSaveGame->setEnabled(true);
}

/// Zmena fazy hry.
///
/// Emituje signal indikujuci zmenu fazy.
/// @see BoardModel::switchPhase();
void BoardView::switchPhase()
{
    BoardModel::switchPhase();
    emit phaseChanged(phase());
}

/// Aktualizuje labely so skore hracov.
void BoardView::updateScores()
{
    for(int i=1; i<=players()->count(); i++)
    {
        QString temp = QString(Players::toString(i).c_str());
        temp += ":\t" + QString::number(players()->score(i));
        _infoLabels[i]->setText(temp);
    }
}

/// Nastavi viditelnost predmetov na dlazdiciach.
///
/// Kazdy hrac vidi len svoj predmet. Ostatne predmety sa skryju.
/// @param pLoot Cislo predmetu, ktore sa neskryje
void BoardView::setLootVisibility(int pLoot)
{
    TileSet *tiles = tileSet();
    TileView *tile;

    for(int i=0; i<tiles->size(); i++)
    {
        tile = dynamic_cast<TileView *>(tiles->at(i));
        if((tile->loot() > 0))
        {
            if((tile->loot() == pLoot))
            {
                tile->setLootVisibility(true);
            }
            else
            {
                tile->setLootVisibility(false);
            }
        }
    }

    if(freeTile()->loot() > 0)
    {
        if(freeTile()->loot() != pLoot)
        {
            dynamic_cast<TileView *>(freeTile())->setLootVisibility(false);
        }
        else
        {
            dynamic_cast<TileView *>(freeTile())->setLootVisibility(true);
        }
    }
}

/// Ulozenie hry.
/// @see BoardModel::save(std::string fName).
void BoardView::save(std::string fName)
{
    std::ofstream file;
    file.open(fName, std::ios_base::out);

    if(file.fail())
    {
        QMessageBox errBox;
        errBox.critical(0, "Error","Could not open file to save!");
        errBox.setFixedSize(500, 200);
        file.clear();
    }
    else
    {
        std::string s = "";

        s = "Size;" + StrOps::iToStr(boardSize());
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
    }
}

/// Nacitanie hry.
/// @see BoardModel::load(std::string fName).
void BoardView::load(std::string fName)
{
    std::ifstream file;
    file.open(fName, std::ios_base::in);
    if(file.fail())
    {
        QMessageBox errBox;
        errBox.critical(0, "Error","Could not open file to load!");
        errBox.setFixedSize(500, 200);
        file.clear();
    }
    else
    {
        std::string s = "";
        std::vector<std::string> ss;
        int i = 0;

        // Size
        int size = -1;
        // Objectives
        std::vector<int> objectives;
        // Players
        int pCount = -1;
        int pCurrent = -1;
        std::vector<int> pPositions;
        std::vector<int> pScores;
        std::vector<int> pObjectives;
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
        std::vector<int> tPlayers;
        int tObjective = 0;

        TileMask tm;

        while(std::getline(file, s))
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
            QMessageBox errBox;
            errBox.critical(0, "Error","Load failed, file may be corrupted!");
            errBox.setFixedSize(500, 200);
        }
        else
        {
            clearBoard();

            setGoal(goal);
            setBoardSize(size);
            setForbidden(forbidden);
            setPhase(phase);
            freePlayers();
            freeObjectives();

            Players *p = new Players(pCount, pCurrent, pPositions, pScores, pObjectives);
            setPlayers(p);

            Objectives *o = new Objectives(objectives);
            setObjectives(o);

            showBoard(tm);
        }
    }
}
