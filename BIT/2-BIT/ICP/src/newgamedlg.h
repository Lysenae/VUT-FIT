// Subor: newgamedlg.h
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Dialog pre nastavenia novej hry

#ifndef NEWGAMEDLG_H
#define NEWGAMEDLG_H

#include <QDialog>

namespace Ui
{
    class NewGameDialog;
}

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(QWidget *parent = 0);
    ~NewGameDialog();

signals:
    void passParams(int, int, int);

private slots:
    void createSubmitted();

private:
    Ui::NewGameDialog *ui;
};

#endif // NEWGAMEDLG_H
