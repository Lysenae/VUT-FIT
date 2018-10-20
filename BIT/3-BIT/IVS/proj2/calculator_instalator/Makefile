.PHONY: gui clean run pack doc test run install uninstall

PYUIC = pyuic5
DOC   = Doxyfile
TEST  = tests.py
ZIP   = xtomci00_xmalik14_xklima22

all: gui

gui:
	$(PYUIC) calcwindow.ui > ui_calcwindow.py

run: gui
	python3 main.py

pack: clean
	sh skript.sh
	#zip -jr $(ZIP).zip IVS-master/*

doc:
	doxygen $(DOC)

test:
	chmod +x $(TEST)
	./$(TEST)

install:
	sudo dpkg -i calculator_1.0-1_all.deb

uninstall:
	sudo dpkg -r calculator

clean:
	rm -rf ui_calcwindow.py
	rm -rf __pycache__
	rm -rf *.zip
