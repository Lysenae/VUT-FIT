// File:   newgamedialog.cpp
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "newgamedialog.h"

///
/// \brief Constructor
///
NewGameDialog::NewGameDialog()
{
    m_main_lt    = new QVBoxLayout();

    m_players_lt = new QHBoxLayout();
    m_theme_lt   = new QHBoxLayout();
    m_size_lt    = new QHBoxLayout();
    m_btns_lt    = new QHBoxLayout();

    m_players_lbl = new QLabel(tr("Players"));
    m_theme_lbl   = new QLabel(tr("Theme"));
    m_size_lbl    = new QLabel(tr("Size"));

    m_players_cb = new QComboBox();
    m_theme_cb   = new QComboBox();
    m_size_cb    = new QComboBox();

    m_create_btn = new QPushButton(tr("Create"));
    m_cancel_btn = new QPushButton(tr("Cancel"));

    m_btns_spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding,
        QSizePolicy::Minimum);

    m_players_lt->addWidget(m_players_lbl);
    m_players_lt->addWidget(m_players_cb);

    m_theme_lt->addWidget(m_theme_lbl);
    m_theme_lt->addWidget(m_theme_cb);

    m_size_lt->addWidget(m_size_lbl);
    m_size_lt->addWidget(m_size_cb);

    m_btns_lt->addSpacerItem(m_btns_spacer);
    m_btns_lt->addWidget(m_create_btn);
    m_btns_lt->addWidget(m_cancel_btn);

    m_main_lt->addLayout(m_players_lt);
    m_main_lt->addLayout(m_size_lt);
    m_main_lt->addLayout(m_theme_lt);
    m_main_lt->addLayout(m_btns_lt);

    QRect scr = QApplication::desktop()->screenGeometry();
    setGeometry(scr.width()/2 - nWidth/2, scr.height()/2 - nHeight/2,
        nWidth, nHeight);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setModal(true);
    setLayout(m_main_lt);

    QObject::connect(m_create_btn, SIGNAL(clicked()), this, SLOT(onCreateClicked()));
    QObject::connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));

    populateComboBoxes();
}

///
/// \brief Fill comboboxes with values and set initial values.
///
void NewGameDialog::populateComboBoxes()
{
    m_players_cb->addItem("1", QVariant(1));
    m_players_cb->addItem("2", QVariant(2));
    m_players_cb->addItem("3", QVariant(3));
    m_players_cb->addItem("4", QVariant(4));
    m_players_cb->setCurrentIndex(0);

    m_theme_cb->addItem(tr("Animals"), QVariant(1));
    m_theme_cb->addItem(tr("Nature"), QVariant(2));
    m_theme_cb->addItem(tr("Fruit"), QVariant(3));
    m_theme_cb->setCurrentIndex(0);

    m_size_cb->addItem("4x4", QVariant(QPoint(4, 4)));
    m_size_cb->addItem("4x5", QVariant(QPoint(4, 5)));
    m_size_cb->addItem("4x6", QVariant(QPoint(4, 6)));
    m_size_cb->addItem("6x6", QVariant(QPoint(6, 6)));
    m_theme_cb->setCurrentIndex(0);
}

///
/// \brief Handler for accepting dialog.
///
void NewGameDialog::onCreateClicked()
{
    int players = m_players_cb->itemData(m_players_cb->currentIndex()).toInt();
    int theme   = m_theme_cb->itemData(m_theme_cb->currentIndex()).toInt();
    QPoint size = m_size_cb->itemData(m_size_cb->currentIndex()).toPoint();
    emit passSettings(players, theme, size);
    accept();
}
