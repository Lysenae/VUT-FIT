// File:   tile.cpp
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "tile.h"

///
/// \brief Constructor
/// \param id
/// \param order
/// \param theme
/// \param parent
///
Tile::Tile(int id, int order, Theme *theme, QWidget *parent)
 : QWidget(parent)
{
    m_id        = id;
    m_flipped   = false;
    m_collected = false;
    m_theme     = theme;
    m_order     = order;

    QPixmap p = QPixmap(m_theme->imageName(BACK)).scaled(100, 100);
    m_pm    = new QPixmap(p);
    m_scene = new QGraphicsScene();
    m_gi    = new QGraphicsPixmapItem(*m_pm);

    m_scene->addItem(m_gi);

    m_view = new QGraphicsView(m_scene);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_layout = new QHBoxLayout();
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_view);
    m_layout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(m_layout);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

///
/// \brief Mouse press event handler.
///
void Tile::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}

///
/// \brief Check if tile is flipped.
/// \return Bool
///
bool Tile::isFlipped()
{
    return m_flipped;
}

///
/// \brief Check if tile is collected.
/// \return Bool
///
bool Tile::isCollected()
{
    return m_collected;
}

///
/// \brief Toggle flipped flag.
///
void Tile::toggleFlipped()
{
    m_flipped = !m_flipped;
    changePixmap();
}

///
/// \brief Set tile's collected flag to true.
///
void Tile::setCollected()
{
    m_collected = true;
    m_flipped   = false;
    changePixmap();
}

///
/// \brief Tile id.
/// \return Id
///
int Tile::id()
{
    return m_id;
}

///
/// \brief Tile order.
/// \return Order
///
int Tile::order()
{
    return m_order;
}

///
/// \brief Change tile's image according to flipped/collected flags.
///
void Tile::changePixmap()
{
    if(m_pm) delete m_pm;

    int id;
    if(m_flipped && !m_collected)
        id = m_id;
    else if(!m_flipped && !m_collected)
        id = BACK;
    else
        id = BLANK;

    QPixmap p = QPixmap(m_theme->imageName(id)).scaled(100, 100);
    m_pm = new QPixmap(p);
    if(m_gi) delete m_gi;
    m_scene->clear();
    m_gi = new QGraphicsPixmapItem(*m_pm);
    m_scene->addItem(m_gi);
    m_view->update();
}
