#-------------------------------------------------
# Autor: Daniel Klimaj (xklima22)
#        Vojtěch Večeřa (xvecer18)
#
# Project created by QtCreator 2015-04-15T15:48:51
#
# Vygenerovane automaticky
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labyrinth2015
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -static-libstdc++

SOURCES += \
    boardview.cpp \
    boardmodel.cpp \
    newgamedlg.cpp \
    tileset.cpp \
    tilemask.cpp \
    tilerotation.cpp \
    tileview.cpp \
    tile.cpp \
    random.cpp \
    players.cpp \
    objectives.cpp \
    vectorops.cpp \
    stringops.cpp \
    reversestep.cpp \
    main_gui.cpp

HEADERS  += \
    boardview.h \
    boardmodel.h \
    newgamedlg.h \
    tileset.h \
    tilemask.h \
    tilerotation.h \
    tileview.h \
    tile.h \
    random.h \
    players.h \
    objectives.h \
    vectorops.h \
    stringops.h \
    reversestep.h

FORMS += \
    boardview.ui \
    newgamedlg.ui

RESOURCES += \
    resources.qrc
