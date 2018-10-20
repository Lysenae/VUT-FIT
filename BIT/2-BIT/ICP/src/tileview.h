// Subor: tileview.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: GUI nadstavba triedy Tile

#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QtCore>
#include <QWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QTransform>

#include "tile.h"

class TileView : public QWidget, public Tile
{
    Q_OBJECT

public:
    TileView(TileType type, int position, unsigned int rotation,
        std::vector<int> players, int phase, int objective);
    ~TileView();

    void setLoot(int newLoot);         // Override
    void addPlayer(int player);        // Override
    void removePlayer(int player);     // Override
    void setActive(bool active);       // Override

    bool lootVisibility() const;
    void setLootVisibility(bool visible);

    void rotate();

    static QString imageByLootId(unsigned int id);

private:
    int                _phase;
    bool               _lootVisibility;

    QHBoxLayout         *_layout;
    QGraphicsScene      *_scene;
    QGraphicsView       *_view;
    QPixmap             *_origPm;
    QPixmap             *_currPm;

    QGraphicsPixmapItem *_giTile;
    QGraphicsPixmapItem *_giLoot;
    QGraphicsPixmapItem *_giPlayerRed;
    QGraphicsPixmapItem *_giPlayerBlue;
    QGraphicsPixmapItem *_giPlayerGreen;
    QGraphicsPixmapItem *_giPlayerYellow;

    void mousePressEvent(QMouseEvent *event);
    QPixmap* setImageByRotation();
    QString imageByTileType(TileType type) const;
    void drawPlayers();
    void drawObjective();
    QSize sizeHint() const;

    int phase() const;
    void setPhase(int phase);

signals:
    void activeChanged();
    void rotationChanged();
    void lootChanged();
    void playersChanged();
    void shiftInitiated(int);
    void moveInitiatedTo(int);
    void pathFindingInitiated(int);
    void lootVisibilityChanged();

private slots:
    void onActiveChanged();
    void onRotationChanged();
    void onLootChanged();
    void onPlayersChanged();
    void onPhaseChanged(int newPhase);
    void onLootVisibilityChanged();
};

#endif // TILEVIEW_H
