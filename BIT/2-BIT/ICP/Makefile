.PHONY = clean cli gui doxygen run pack

PROJECT = labyrinth2015
CLI = $(PROJECT)-cli

CLI_SOURCES = \
	src/main_cli.cpp\
	src/boardmodel.cpp \
	src/tileset.cpp \
	src/tilemask.cpp \
	src/tilerotation.cpp \
	src/tile.cpp \
	src/random.cpp \
	src/players.cpp \
	src/objectives.cpp \
	src/vectorops.cpp \
	src/stringops.cpp \
	src/reversestep.cpp

CXX = $(shell command -v g++-4.8 >/dev/null 2>&1 && echo -n "g++-4.8" || echo -n "g++")

CFLAGS = -Wall -Wextra --std=c++11 -O2
LFLAGS = $(shell command -v g++-4.8 >/dev/null 2>&1 && echo -n "-static-libstdc++")

ARCHIVE_FILES = src/* examples/* Makefile README doxyconfig
ARCHIVE = xklima22-xvecer18.zip

all: cli gui

cli:
	$(CXX) $(CFLAGS) $(CLI_SOURCES) -o src/$(CLI) $(LFLAGS)

gui:
	cd src && qmake
	cd src && make

doxygen:
	doxygen doxyconfig

run:
	(cd src && exec ./labyrinth2015)
	(cd src && exec ./labyrinth2015-cli)

pack: clean
	zip -r $(ARCHIVE) $(ARCHIVE_FILES)

clean:
	rm -f src/*.o
	rm -f src/ui_*.h
	rm -f src/moc_*.cpp
	rm -f src/qrc_*
	rm -f src/$(PROJECT)
	rm -f src/$(CLI)
	rm -f src/Makefile
	rm -f src/*.user
	rm -rf doc
	rm -f doxyconfig.bak