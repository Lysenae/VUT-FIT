// File:   board.h
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>
#include <QStringList>

#include "dimension.h"
#include "theme.h"
#include "tile.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(Dimension *dim, Theme *theme, QWidget *parent = 0);
    ~Board();
    virtual void mousePressEvent(QMouseEvent *);
    void createBoard();
    QVector<QString> toStrings();
    void setFirstSelected(int i);
    void setSecondSelected(int i);
    void createLoadedBoard(QVector<QVector<int> > tv, int t1, int t2);

private:
    Dimension *m_dim;
    Theme     *m_theme;

    Tile *m_first;
    Tile *m_second;

    int m_pairs;

    QVector<Tile*>  m_tiles;

    QVBoxLayout          *m_rows;
    QVector<QHBoxLayout*> m_cols;

    void shuffleVector(QVector<int> *v);
    void hideFlippedTiles();

signals:
    void endTurn(bool inc_score);
    void gameOver();

private slots:
    void onTileClicked();
};

#endif // BOARD_H
