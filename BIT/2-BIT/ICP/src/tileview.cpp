// Subor: tileview.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: GUI nadstavba triedy Tile

#include "tileview.h"

/// Konstruktor.
///
/// @param type Typ dlazdice.
/// @param position Pozicia v hracej ploche alebo -1 ak je volna dlazdica.
/// @param rotation Stupen natocenia (1-4).
/// @param players Zoznam hracov na dlazdici.
/// @param phase Faza hry (0,1).
/// @param objective Cislo ulohy na dlazdici (0 ak ziadna nie je).
TileView::TileView(TileType type, int position, unsigned int rotation,
std::vector<int> players, int phase, int objective) :
    Tile(type, position, rotation, players, objective)
{
    _phase          = phase;
    _lootVisibility = true;

    _giTile         = NULL;
    _giLoot         = NULL;
    _giPlayerBlue   = NULL;
    _giPlayerRed    = NULL;
    _giPlayerGreen  = NULL;
    _giPlayerYellow = NULL;

    QPixmap temp = QPixmap(imageByTileType(tileType())).scaled(sizeHint().width(), sizeHint().height());
    _origPm = new QPixmap(temp);
    _currPm = setImageByRotation();

    _scene = new QGraphicsScene();
    _giTile = new QGraphicsPixmapItem(*_currPm);
    _scene->addItem(_giTile);

    _view = new QGraphicsView(_scene);
    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->fitInView(0, 0, _origPm->width()*10, _origPm->height(), Qt::KeepAspectRatio);

    _layout = new QHBoxLayout();
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);
    _layout->addWidget(_view);
    _layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(_layout);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    drawPlayers();
    drawObjective();
    onActiveChanged();
    setPaths();

    QObject::connect(this, SIGNAL(activeChanged()), this, SLOT(onActiveChanged()));
    QObject::connect(this, SIGNAL(rotationChanged()), this, SLOT(onRotationChanged()));
    QObject::connect(this, SIGNAL(lootChanged()), this, SLOT(onLootChanged()));
    QObject::connect(this, SIGNAL(playersChanged()), this, SLOT(onPlayersChanged()));
    QObject::connect(this, SIGNAL(lootVisibilityChanged()), this, SLOT(onLootVisibilityChanged()));
}

/// Destruktor.
TileView::~TileView()
{
}

/// Nastavi ulohu dlazdice.
///
/// Nastavi ulohu na dlazdici a v pripade uspechu emituje signal.
/// Ulohu mozeme len pridat alebo odstranit, nemozeme nehradit existujucu
/// ulohu novou ulohou.
/// @param newLoot Cislo novej ulohy.
void TileView::setLoot(int newLoot)
{
    if(((loot() == 0) && (newLoot > 0)) || ((loot() > 0) && (newLoot == 0)))
    {
        if(loot() != newLoot)
        {
            Tile::setLoot(newLoot);
            emit lootChanged();
        }
    }
}

/// Prida hraca.
///
/// Prida hraca na dlazdicu a emituje signal.
/// @param player Cislo hraca.
void TileView::addPlayer(int player)
{
    Tile::addPlayer(player);
    emit playersChanged();
}

/// Odstrani hraca.
///
/// Odstrani hraca z dlazdice a emituje signal.
/// @param player Cislo hraca.
void TileView::removePlayer(int player)
{
    Tile::removePlayer(player);
    emit playersChanged();
}

/// Nastavi aktivity dlazdice.
///
/// @param active Bool.
void TileView::setActive(bool active)
{
    Tile::setActive(active);
    emit activeChanged();
}

/// Je predmet (uloha) na dlazdici viditelna pre akutalneho hraca?
///
/// @return Bool.
bool TileView::lootVisibility() const
{
    return _lootVisibility;
}

/// Nastavi viditelnost ulohy pre aktualneho hraca.
///
/// @param visible Bool.
void TileView::setLootVisibility(bool visible)
{
    if(_lootVisibility != visible)
    {
        _lootVisibility = visible;
        emit lootVisibilityChanged();
    }
}

/// Vrati cestu k obrazku predmetu na zaklade jeho cisla.
///
/// @param id Cislo ulohy.
/// @return Cestu k obrazku predmetu.
QString TileView::imageByLootId(unsigned int id)
{
    QString str;
    if(id > 0 && id <= 24)
    {
        str = ":/gems/gem" + QString::number(id);
    }
    else
    {
        str = "";
    }

    return str;
}

/// Aktualna faza.
///
/// @return Cislo fazy, 1 alebo 0.
int TileView::phase() const
{
    return _phase;
}

/// Nastavi fazu.
///
/// @param phase Cislo fazy.
void TileView::setPhase(int phase)
{
    _phase = phase;
}

/// Handler signalu activeChanged.
///
/// NAstavuje pozadie a kurzor dlazdice podla aktivity.
void TileView::onActiveChanged()
{
    if(active())
    {
        setCursor(Qt::PointingHandCursor);
        _view->setBackgroundBrush(QBrush(QColor::fromRgbF(0, 1, 0)));
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        _view->setBackgroundBrush(QBrush(QColor::fromRgbF(1, 0, 0)));
    }
}

/// Handler signalu o zmene rotacie.
///
/// Otoci obrazok a nastavi cesty.
void TileView::onRotationChanged()
{
    _currPm = setImageByRotation();
    if(_giTile)
    {
        delete _giTile;
    }

    if(_giLoot)
    {
        delete _giLoot;
    }

    _scene->clear();

    _giTile = new QGraphicsPixmapItem(*_currPm);
    _scene->addItem(_giTile);

    // Treba doplnit aj predmet??
    if(loot() > 0)
    {
        _giLoot = new QGraphicsPixmapItem(TileView::imageByLootId(loot()));
        if(lootVisibility())
        {
            _scene->addItem(_giLoot);
        }
    }

    _view->update();
    setPaths();
}

/// Handler signalu o zmene ulohy.
///
/// Nastavi alebo odstrani obrazok predmetu z dlazdice.
void TileView::onLootChanged()
{
    if(loot() == 0)
    {
        if(_giLoot)
        {
            QList<QGraphicsItem *> items = _scene->items();
            for(int i=0; i<items.size(); i++)
            {
                if(items[i] == _giLoot)
                {
                    _scene->removeItem(_giLoot);
                    break;
                }
            }
            delete _giLoot;
            _giLoot = NULL;
        }
    }
    else
    {
        drawObjective();
    }
}

/// Handler signalu o zmene hracov na dlazdici.
///
/// Vykresli alebo odstrani obrazok figurky hraca z dlazdice.
void TileView::onPlayersChanged()
{
    drawPlayers();
}

/// Handler signalu o zmene fazy hracej plochy.
///
/// Nastavi hodnotu fazy dlazdice, aby zodpovedala fazy hracej plochy.
void TileView::onPhaseChanged(int newPhase)
{
    setPhase(newPhase);
}

/// Handler signalu o zmene viditelnosti predmetu.
void TileView::onLootVisibilityChanged()
{
    if(!lootVisibility())
    {
        _scene->removeItem(_giLoot);
    }
    else
    {
        if(_giLoot != NULL)
        {
            _scene->addItem(_giLoot);
        }
    }
}

/// Spravuje udalosti kliknutia na dlazdicu.
void TileView::mousePressEvent(QMouseEvent *event)
{
    if(active())
    {
        if(event->button() == Qt::LeftButton)
        {
            if(rotatable())
            {
                rotate();
            }
            else
            {
                if(phase() == 0)
                {
                    emit shiftInitiated(position());
                }
                else
                {
                    emit moveInitiatedTo(position());
                }
            }
        }
    }
}

/// Zmena rotacie.
///
/// Emituje signal o zmene rotacie.
void TileView::rotate()
{
    rotationNext();
    emit rotationChanged();
}

/// Nastavi obrazok podla stupna rotacie.
///
/// @return Pixmapu natoceneho obrazka.
QPixmap *TileView::setImageByRotation()
{
    QTransform tr;
    tr.translate(_origPm->width()/2,_origPm->height()/2);
    tr.rotate(90 * (rotation()-1));
    tr.translate(-_origPm->width()/2,-_origPm->height()/2);
    return new QPixmap(_origPm->transformed(tr));
}


/// Vrati cestu k obrazku dlazdice podla zadaneho typu.
///
/// @param type Typ dlazdice (T, L, I).
/// @return Retazec s cestou k obrazku.
QString TileView::imageByTileType(TileType type) const
{
    QString fn;
    switch(type)
    {
        case T_Tile:
            fn = ":/tiles/t_tile";
            break;
        case L_Tile:
            fn = ":/tiles/l_tile";
            break;
        case I_Tile:
            fn = ":/tiles/i_tile";
            break;
        default:
            fn = "";
            break;
    }

    return fn;
}

/// Vykresli hracov.
void TileView::drawPlayers()
{
    if(_giPlayerBlue != NULL)
    {
        _scene->removeItem(_giPlayerBlue);
        delete _giPlayerBlue;
        _giPlayerBlue = NULL;
    }

    if(_giPlayerRed != NULL)
    {
        _scene->removeItem(_giPlayerRed);
        delete _giPlayerRed;
        _giPlayerRed = NULL;
    }

    if(_giPlayerGreen != NULL)
    {
        _scene->removeItem(_giPlayerGreen);
        delete _giPlayerGreen;
        _giPlayerGreen = NULL;
    }

    if(_giPlayerYellow != NULL)
    {
        _scene->removeItem(_giPlayerYellow);
        delete _giPlayerYellow;
        _giPlayerYellow = NULL;
    }

    std::vector<int> pl = playerList();
    int count           = pl.size();
    int y;

    if(count > 0)
    {
        for(int i=0, x=0; i<count && i<4; i++, x+=15)
        {
            y = i < 3 ? 35 : 20;
            if(i == 3)
            {
                x = 0;
            }

            if(pl[i] == 1)
            {
                _giPlayerBlue = new QGraphicsPixmapItem(QPixmap(":/players/blue"));
                _giPlayerBlue->setPos(x, y);
                _scene->addItem(_giPlayerBlue);
            }
            else if(pl[i] == 2)
            {
                _giPlayerRed = new QGraphicsPixmapItem(QPixmap(":/players/red"));
                _giPlayerRed->setPos(x, y);
                _scene->addItem(_giPlayerRed);
            }
            else if(pl[i] == 3)
            {
                _giPlayerGreen = new QGraphicsPixmapItem(QPixmap(":/players/green"));
                _giPlayerGreen->setPos(x, y);
                _scene->addItem(_giPlayerGreen);
            }
            else if(pl[i] == 4)
            {
                _giPlayerYellow = new QGraphicsPixmapItem(QPixmap(":/players/yellow"));
                _giPlayerYellow->setPos(x, y);
                _scene->addItem(_giPlayerYellow);
            }
        }
    }
}

/// Vykresli predmety.
void TileView::drawObjective()
{
    if(_giLoot != NULL)
    {
        _scene->removeItem(_giLoot);
        delete _giLoot;
        _giLoot = NULL;
    }

    QString lootImg = TileView::imageByLootId(loot());
    if(lootImg != "") // "" sa vracia pri chybnom ID
    {
        _giLoot = new QGraphicsPixmapItem(QPixmap(lootImg));
        _scene->addItem(_giLoot);
    }
}

/// Rozmery.
QSize TileView::sizeHint() const
{
    return QSize(50, 50);
}
