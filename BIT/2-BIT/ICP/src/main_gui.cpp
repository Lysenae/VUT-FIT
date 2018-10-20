// Subor: main_gui.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Main pre GUI verziu hry

#include "boardview.h"
#include <QApplication>
#include <QStringList>
#include <QString>

/// Main pre GUI verziu.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = a.arguments();

    QString arg = "";
    if(args.count() == 3)
    {
        if(args[1] == "--load")
        {
            arg = args[2];
        }
    }

    BoardView w(arg);
    w.show();

    return a.exec();
}
