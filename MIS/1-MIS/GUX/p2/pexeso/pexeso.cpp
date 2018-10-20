// File:   pexeso.cpp
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "pexeso.h"

///
/// \brief Constructor.
/// \param parent
///
Pexeso::Pexeso(QWidget *parent) : QMainWindow(parent)
{
    QResource::registerResource(QString("images.rcc"));

    m_ngd = new NewGameDialog();

    m_dim     = new Dimension(0, 0);
    m_players = new Players();
    m_theme   = new Theme();
    m_window  = new QWidget();
    m_game    = false;
    QRect scr = QApplication::desktop()->screenGeometry();
    setGeometry(scr.width()/2 - nWidth/2, scr.height()/2 - nHeight/2,
        nWidth, nHeight);
    setWindowTitle(tr("Pexeso"));
    setMinimumSize(nWidth, nHeight);

    createMenu();

    QObject::connect(m_ngd, SIGNAL(passSettings(int, int, QPoint)),
        this, SLOT(getSettings(int, int, QPoint)));
}

///
/// \brief Destructor.
///
Pexeso::~Pexeso()
{
    clearLayout();
}

///
/// \brief Close event handler.
/// \param e
///
void Pexeso::closeEvent(QCloseEvent *e)
{
    if(m_game)
    {
        QMessageBox::StandardButton r;
        QMessageBox q;
        r = q.question(0, "Quit?", "Are you sure you want to quit?",
            QMessageBox::Yes|QMessageBox::No);
        q.setFixedSize(500, 200);
        if(r == QMessageBox::No)
        {
            e->ignore();
        }
    }
}

///
/// \brief Create menu.
///
void Pexeso::createMenu()
{
    m_new_action = new QAction(tr("&New ..."), this);
    m_new_action->setShortcuts(QKeySequence::New);
    m_new_action->setStatusTip(tr("New game"));
    QObject::connect(m_new_action, SIGNAL(triggered()), this, SLOT(onNewGame()));

    m_save_action = new QAction(tr("&Save ..."), this);
    m_save_action->setShortcuts(QKeySequence::Save);
    m_save_action->setStatusTip(tr("Save game"));
    m_save_action->setEnabled(false);
    QObject::connect(m_save_action, SIGNAL(triggered()), this, SLOT(onSaveGame()));

    m_load_action = new QAction(tr("&Load ..."), this);
    m_load_action->setShortcuts(QKeySequence::Open);
    m_load_action->setStatusTip(tr("Load game"));
    QObject::connect(m_load_action, SIGNAL(triggered()), this, SLOT(onLoadGame()));

    m_quit_action = new QAction(tr("E&xit"), this);
    m_quit_action->setShortcuts(QKeySequence::Close);
    m_quit_action->setStatusTip(tr("Load game"));
    QObject::connect(m_quit_action, SIGNAL(triggered()), this, SLOT(close()));

    m_game_menu = menuBar()->addMenu(tr("&Game"));
    m_game_menu->addAction(m_new_action);
    m_game_menu->addAction(m_save_action);
    m_game_menu->addAction(m_load_action);
    m_game_menu->addAction(m_quit_action);
}

///
/// \brief Create layout.
///
void Pexeso::createLayout()
{
    // Info panel
    m_info_lt  = new QVBoxLayout();

    m_players_ld = new QLabel(QString(tr("Players:")), m_window);
    m_players_lv = new QLabel(QString::number(m_players->count()), m_window);
    m_theme_ld   = new QLabel(QString(tr("Theme:")), m_window);
    m_theme_lv   = new QLabel(m_theme->toString(), m_window);
    m_size_ld    = new QLabel(QString(tr("Size:")), m_window);
    m_size_lv    = new QLabel(m_dim->toString(), m_window);
    m_cplayer_ld = new QLabel(QString(tr("Player's turn:")), m_window);
    m_cplayer_lv = new QLabel(QString::number(m_players->current()), m_window);

    m_info_w_main  = new QWidget(m_window);
    m_info_w_main->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    m_info_lt_main = new QHBoxLayout();
    m_info_lt_desc = new QVBoxLayout();
    m_info_lt_val  = new QVBoxLayout();

    m_info_lt_desc->addWidget(m_players_ld);
    m_info_lt_desc->addWidget(m_size_ld);
    m_info_lt_desc->addWidget(m_theme_ld);
    m_info_lt_desc->addWidget(m_cplayer_ld);

    m_info_lt_val->addWidget(m_players_lv);
    m_info_lt_val->addWidget(m_size_lv);
    m_info_lt_val->addWidget(m_theme_lv);
    m_info_lt_val->addWidget(m_cplayer_lv);

    m_info_lt_main->addLayout(m_info_lt_desc);
    m_info_lt_main->addLayout(m_info_lt_val);
    m_info_w_main->setLayout(m_info_lt_main);

    m_info_lt->addWidget(m_info_w_main);
    m_info_lt->addSpacing(5);

    m_scores_w = new QWidget();
    m_scores_w->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    m_scores_lt = new QVBoxLayout();
    m_scores_lt->setAlignment(Qt::AlignLeft);

    QString temp;
    for(int i=0; i<m_players->count(); ++i)
    {
        temp = QString("Player " + QString::number(i+1) + ": " +
            QString::number(m_players->getScore(i+1)));
        m_scores.append(new QLabel(temp, m_window));
    }

    for(int i=0; i<m_scores.size(); ++i)
    {
        m_scores_lt->addWidget(m_scores[i], 0, Qt::AlignTop);
    }

    m_scores_w->setLayout(m_scores_lt);
    m_info_lt->addWidget(m_scores_w);

    m_info_lt->addStretch();

    m_info_w = new QWidget();
    m_info_w->setGeometry(m_info_w->geometry().x(), m_info_w->geometry().y(),
        250, m_info_w->geometry().height());
    m_info_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    m_info_w->setLayout(m_info_lt);

    // Main layout
    m_main_lt  = new QHBoxLayout();
    m_main_lt->addWidget(m_info_w);

    m_window->setLayout(m_main_lt);

    setCentralWidget(m_window);
}

///
/// \brief Pexeso::Create board.
///
void Pexeso::createBoard()
{
    m_board_w = new Board(m_dim, m_theme);
    QPalette Pal(palette());

    Pal.setColor(QPalette::Background, Qt::black);
    m_board_w->setAutoFillBackground(true);
    m_board_w->setPalette(Pal);
    m_board_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_main_lt->addWidget(m_board_w);

    QObject::connect(m_board_w, SIGNAL(endTurn(bool)), this, SLOT(onEndTurn(bool)));
    QObject::connect(m_board_w, SIGNAL(gameOver()), this, SLOT(onGameOver()));
}

///
/// \brief Clear layout.
///
void Pexeso::clearLayout()
{
    if(m_window != NULL)
    {
        delete m_window;
        m_window = new QWidget();
        setCentralWidget(NULL);
    }

    m_scores.clear();
}

///
/// \brief Create new game.
///
void Pexeso::createNewGame()
{
    createLayout();
    createBoard();

    m_board_w->createBoard();
    int w = m_info_w->width()+m_board_w->minimumWidth()+50;
    int h = m_board_w->minimumHeight()+50;
    setMinimumSize(w, h);
    resize(w, h);
    m_save_action->setEnabled(true);
    m_game = true;
}

///
/// \brief Save current game.
/// \param file
///
void Pexeso::save(QFile *file)
{
    QTextStream stream(file);
    QStringList l;

    QVector<QString> board = m_board_w->toStrings();

    l << "players";
    l << QString::number(m_players->count());
    l << QString::number(m_players->current());
    stream << l.join(";") << "\n";
    l.clear();

    for(int i=1; i<=m_players->count(); ++i)
    {
        l << "score";
        l << QString::number(i);
        l << QString::number(m_players->getScore(i));
        stream << l.join(";") << "\n";
        l.clear();
    }

    l << "size";
    l << QString::number(m_dim->rows());
    l << QString::number(m_dim->cols());
    stream << l.join(";") << "\n";
    l.clear();

    l << "theme";
    l << QString::number((int)m_theme->get());
    stream << l.join(";") << "\n";
    l.clear();

    for(int i=0; i<board.size(); ++i)
    {
        stream << board[i] << "\n";
    }
}

///
/// \brief Load saved game.
/// \param lines Lines of file
/// \return
///
bool Pexeso::load(QStringList lines)
{
    clearLayout();

    QStringList parts;
    QVector<QVector<int> > tv;
    QVector<int> tvh;
    QString line;

    bool c_players = false;
    bool c_size    = false;
    bool c_theme   = false;
    bool c_select  = false;

    int l, m, n, s, ct, cs, t1, t2;
    bool cl, cm, cn;

    t1 = t2 = -1;
    cs = ct = -1;
    s = 0;

    for(int i=0; i<lines.size(); ++i)
    {
        line = lines[i].trimmed();
        parts = line.split(';');

        if(parts[0] == "players" && parts.size() == 3)
        {
            l = parts[1].toInt(&cl);
            m = parts[2].toInt(&cm);
            if(cl && cm)
            {
                m_players->setCount(l);
                m_players->setCurrent(m);
                c_players = true;
                cs = l;
            }
        }
        else if(parts[0] == "score" && parts.size() == 3)
        {
            l = parts[1].toInt(&cl);
            m = parts[2].toInt(&cm);
            if(cl && cm)
            {
                m_players->setScore(l, m);
                s++;
            }
        }
        else if(parts[0] == "size" && parts.size() == 3)
        {
            l = parts[1].toInt(&cl);
            m = parts[2].toInt(&cm);
            if(cl && cm)
            {
                m_dim->set(l, m);
                c_size = true;
                ct = l*m;
            }
        }
        else if(parts[0] == "theme" && parts.size() == 2)
        {
            l = parts[1].toInt(&cl);
            if(cl)
            {
                m_theme->set(l);
                c_theme = true;
            }
        }
        else if(parts[0] == "tile")
        {
            l = parts[1].toInt(&cl);
            m = parts[2].toInt(&cm);
            n = parts[3].toInt(&cn);
            if(cl && cm && cn)
            {
                tvh.clear();
                tvh << l << m << n;
                tv << tvh;
            }
        }
        else if(parts[0] == "select" && parts.size() == 3)
        {
            l = parts[1].toInt(&cl);
            m = parts[2].toInt(&cm);
            if(cl && cm)
            {
                t1 = l;
                t2 = m;
                c_select = true;
            }
        }
        else
        {
            return false;
        }
    }

    if(c_players && c_size && c_theme && c_select && (s == cs) &&
    (tv.size() == ct))
    {
        createLayout();
        createBoard();
        m_board_w->createLoadedBoard(tv, t1, t2);
        int w = m_info_w->width()+m_board_w->minimumWidth()+50;
        int h = m_board_w->minimumHeight()+50;
        setMinimumSize(w, h);
        resize(w, h);
        m_save_action->setEnabled(true);
        m_game = true;
        return true;
    }

    return false;
}

///
/// \brief Reads file line by line.
/// \param file
/// \return File lines
///
QStringList Pexeso::fileToStringList(QFile *file)
{
    QStringList rslt;
    while(!file->atEnd())
        rslt << file->readLine().trimmed();
    return rslt;
}

void Pexeso::showErrorMsg(QString msg)
{
    QMessageBox err_box;
    err_box.critical(0, "Error", msg);
    err_box.setFixedSize(500, 200);
}

void Pexeso::getSettings(int players, int theme, QPoint size)
{
    m_players->setCount(players);
    m_theme->set(theme);
    m_dim->set(size.x(), size.y());
}

void Pexeso::onNewGame()
{
    if(m_ngd->exec() == QDialog::Accepted)
    {
        clearLayout();
        createNewGame();
    }
}

bool Pexeso::onSaveGame()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save ..."),
        QString(), tr("Pexeso Save File (*.pxs);;All Files (*.*)"));
    if(!fn.isEmpty())
    {
        QFile file(fn);
        if(file.open(QIODevice::WriteOnly))
        {
            save(&file);
            file.close();
            return true;
        }
        else
        {
            showErrorMsg("Could not open file to write!");
            return false;
        }
    }

    return false;
}

bool Pexeso::onLoadGame()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Load ..."),
            QString(), tr("Pexeso Save File (*.pxs);;All Files (*.*)"));

    if(!fn.isEmpty())
    {
        QFile file(fn);
        if(file.open(QIODevice::ReadOnly))
        {
            bool status = load(fileToStringList(&file));
            if(!status)
            {
                showErrorMsg("Corrupted save file!");
                return false;
            }
            file.close();
            return true;
        }
        else
        {
            showErrorMsg("Could not open file to read!");
            return false;
        }
    }

    return false;
}

void Pexeso::onEndTurn(bool inc_score)
{
    if(inc_score)
    {
        m_players->incScore(m_players->current());
    }
    m_players->next();

    for(int i=0; i<m_scores.size(); ++i)
    {
        QString temp = QString("Player " + QString::number(i+1) + ": " +
            QString::number(m_players->getScore(i+1)));
        m_scores[i]->setText(temp);
        m_cplayer_lv->setText(QString::number(m_players->current()));
        m_info_w->repaint();
    }
}

void Pexeso::onGameOver()
{
    QVector<int> w = m_players->getWinners();
    QString s = "Player";
    s += w.size() > 1 ? "s " : " ";

    for(int i=0; i<w.size(); ++i)
    {
        s += QString::number(w[i]);
        if(i<w.size()-1)
            s += ", ";
    }
    s += " won!";

    QMessageBox msg_box;
    msg_box.information(0, "Game Over", s);
    msg_box.setFixedSize(500, 200);
    clearLayout();
    m_save_action->setEnabled(false);
    m_game = false;
}
