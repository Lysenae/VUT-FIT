// File:   board.cpp
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "board.h"

///
/// \brief Constructor
///
Board::Board(Dimension *dim, Theme *theme, QWidget *parent) : QWidget(parent)
{
    m_dim    = dim;
    m_theme  = theme;
    m_first  = NULL;
    m_second = NULL;
    m_pairs  = m_dim->size()/2;
    setMinimumSize(m_dim->cols()*105, m_dim->rows()*105);
}

///
/// \brief Destructor
///
Board::~Board()
{
}

///
/// \brief Overriden mousePressEvent handler. Params are ignored.
///
void Board::mousePressEvent(QMouseEvent*)
{
    if(m_first != NULL && m_second != NULL)
    {
        hideFlippedTiles();
    }
}

///
/// \brief Creates new game board according to given settings (theme, size).
///
void Board::createBoard()
{
    m_rows = new QVBoxLayout();
    m_rows->setSpacing(5);

    QVector<int> v;
    for(int i=1; i<=m_dim->size()/2; ++i)
    {
        v.append(i);
        v.append(i);
    }
    shuffleVector(&v);
    shuffleVector(&v);
    shuffleVector(&v);
    shuffleVector(&v);
    shuffleVector(&v);

    for(int i=0; i<m_dim->rows(); ++i)
    {
        QHBoxLayout *tmp = new QHBoxLayout();
        m_rows->addLayout(tmp);
        m_cols.append(tmp);
    }

    for(int i=0; i<m_dim->size(); ++i)
    {
        Tile *t = new Tile(v[i], i, m_theme, this);
        m_tiles.append(t);
        m_cols[i / m_dim->cols()]->addWidget(t);
        QObject::connect(t, SIGNAL(clicked()), this, SLOT(onTileClicked()));
    }

    setLayout(m_rows);
}

///
/// \brief Shuffles values in given vector
/// \param v Shuffled vector pointer.
///
void Board::shuffleVector(QVector<int> *v)
{
    int i = v->size();
    int j, t;
    while(i > 1)
    {
        i = i - 1;
        qsrand(QTime::currentTime().msec());
        j = qrand() % i;
        t = v->at(i);
        v->replace(i, v->at(j));
        v->replace(j, t);
    }
}

///
/// \brief Show backs of all not collected tiles.
///
void Board::hideFlippedTiles()
{
    if(m_first->id() == m_second->id())
    {
        m_first->setCollected();
        m_second->setCollected();
        m_first  = NULL;
        m_second = NULL;
        m_pairs--;
        emit endTurn(true);
        if(m_pairs <= 0)
        {
            emit gameOver();
        }
    }
    else
    {
        m_first->toggleFlipped();
        m_second->toggleFlipped();
        m_first  = NULL;
        m_second = NULL;
        emit endTurn(false);
    }
}

///
/// \brief Creates CSV representation of Board.
/// \return CSV representation of Board
///
QVector<QString> Board::toStrings()
{
    QVector<QString> vs;
    QStringList l;
    Tile *t = NULL;

    l.clear();

    for(int i=0; i<m_tiles.size(); ++i)
    {
        t = m_tiles[i];
        l << "tile";
        l << QString::number(t->id());
        l << QString::number(t->isFlipped() ? 1 : 0);
        l << QString::number(t->isCollected() ? 1 : 0);
        vs.append(l.join(";"));
        l.clear();
    }

    l << "select";
    l << QString::number((m_first == NULL) ? -1 : m_first->order());
    l << QString::number((m_second == NULL) ? -1 : m_second->order());
    vs.append(l.join(";"));

    return vs;
}

///
/// \brief Set first selected tile.
/// \param i Index of tile in board.
///
void Board::setFirstSelected(int i)
{
    if(i>=0 && i<m_tiles.size())
        m_first = m_tiles[i];
    else
        m_second = NULL;
}

///
/// \brief Set second selected tile.
/// \param i Index of tile in board.
///
void Board::setSecondSelected(int i)
{
    if(i>=0 && i<m_tiles.size())
        m_second = m_tiles[i];
    else
        m_second = NULL;
}

///
/// \brief Creates the board according to given params
/// \param tv Tiles - ID, flipped flag, collected flag
/// \param t1 First selected tile index
/// \param t2 Second selected tile index
///
void Board::createLoadedBoard(QVector<QVector<int> > tv, int t1, int t2)
{
    m_rows = new QVBoxLayout();
    m_rows->setSpacing(5);

    for(int i=0; i<m_dim->rows(); ++i)
    {
        QHBoxLayout *tmp = new QHBoxLayout();
        m_rows->addLayout(tmp);
        m_cols.append(tmp);
    }

    int c = 0;
    for(int i=0; i<tv.size(); ++i)
    {
        Tile *t = new Tile(tv[i][0], i, m_theme, this);
        if(tv[i][1] == 1) t->toggleFlipped();
        if(tv[i][2] == 1)
        {
            t->setCollected();
            c++;
        }
        if(i == t1) m_first  = t;
        if(i == t2) m_second = t;
        m_tiles.append(t);
        m_cols[i / m_dim->cols()]->addWidget(t);
        QObject::connect(t, SIGNAL(clicked()), this, SLOT(onTileClicked()));
    }

    m_pairs -= (c/2);
    setLayout(m_rows);
}

///
/// \brief Tile clicked handler
///
void Board::onTileClicked()
{
    Tile *t = qobject_cast<Tile *>(QObject::sender());


    if(m_first == NULL)
    {
        if(!t->isFlipped() && !t->isCollected())
        {
            m_first = t;
            t->toggleFlipped();
        }
    }
    else if(m_second == NULL)
    {
        if(!t->isFlipped() && !t->isCollected())
        {
            m_second = t;
            t->toggleFlipped();
        }
    }
    else
    {
        hideFlippedTiles();
    }
}
