// File:        editor.cpp
// Author:      Daniel Klimaj (xklima22)
// Description: Main file.

#include "mainwindow.h"

int main(int argc, char **argv)
{
  MainWindow *mw = new MainWindow(argc, argv);
  mw->run();
  return 0;
}
