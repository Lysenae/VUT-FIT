// File:   tile.h
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "theme.h"

class Tile : public QWidget
{
    Q_OBJECT

public:
    explicit Tile(int id, int order, Theme *theme, QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *);
    bool isFlipped();
    bool isCollected();
    void toggleFlipped();
    void setCollected();
    int id();
    int order();

private:
    static const int BACK  = 0;
    static const int BLANK = -1;

    int     m_id;
    bool    m_flipped;
    bool    m_collected;
    Theme  *m_theme;
    int     m_order;

    QPixmap             *m_pm;
    QGraphicsScene      *m_scene;
    QGraphicsView       *m_view;
    QGraphicsPixmapItem *m_gi;

    QHBoxLayout *m_layout;

    void changePixmap();

signals:
    void clicked();

public slots:
};

#endif // TILE_H
