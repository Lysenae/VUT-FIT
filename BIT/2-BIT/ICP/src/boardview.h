// Subor: boardview.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: BoardView GUI nadstavba triedy BoardModel
//        UI bolo vygenerovane automaticky

#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLayout>
#include <QVector>
#include <QLayoutItem>
#include <QScrollArea>
#include <QDesktopWidget>
#include <QLabel>
#include <QFileDialog>

#include "boardmodel.h"

// Specificke pre GUI
#include "newgamedlg.h"
#include "tileview.h"

namespace Ui
{
    class BoardView;
}

class BoardView : public QMainWindow, public BoardModel
{
    Q_OBJECT

public:
    explicit BoardView(QString fPath = "", QWidget *parent = 0);
    ~BoardView();

    void load(std::string fName);

private slots:
    void newGame();
    void createGame(int, int, int);
    void handleShiftTiles(int pos);
    void handleMoveTo(int pos);
    void handlePathFinding(int pos);
    void handleSave();
    void handleLoad();
    void handleUndo();

private:
    Ui::BoardView *ui;

    QSize _geometry;

    NewGameDialog *_newGameDlg;
    QWidget       *_w;
    QHBoxLayout   *_mainLayout;
    QVBoxLayout   *_rows;
    QVBoxLayout   *_infoLayout;
    QLabel        *_ftInfo;
    QLabel        *_objInfo;
    QLabel        *_pLoot;

    QVector< QLabel *>      _infoLabels;
    QVector< QHBoxLayout *> _cols;

    void showBoard(TileMask tm);
    void clearBoard();
    void switchPhase();
    void updateScores();
    void setLootVisibility(int pLoot);
    void save(std::string fName);

signals:
    void phaseChanged(int);
};

#endif // BOARDVIEW_H
