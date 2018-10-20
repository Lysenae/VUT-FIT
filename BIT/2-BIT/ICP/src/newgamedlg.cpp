// Subor: newgamedlg.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Dialog pre nastavenia novej hry
//        UI bolo vygenerovane automaticky (newgamedlg.ui)

#include "newgamedlg.h"
#include "ui_newgamedlg.h"

/// Konstruktor.
NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->ngdBtnCreate, SIGNAL(clicked()), this, SLOT(createSubmitted()));
    ui->ngdCbSize->setCurrentIndex(1);
}

/// Destruktor.
NewGameDialog::~NewGameDialog()
{
    delete ui;
}

/// Akceptovanie nastaveni.
void NewGameDialog::createSubmitted()
{
    int size       = ui->ngdCbSize->itemText(ui->ngdCbSize->currentIndex()).toInt();
    int players    = ui->ngdCbPlayers->itemText(ui->ngdCbPlayers->currentIndex()).toInt();
    int objectives = ui->ngdCbObj->itemText(ui->ngdCbObj->currentIndex()).toInt();
    emit passParams(size, players, objectives);
    accept();
}
