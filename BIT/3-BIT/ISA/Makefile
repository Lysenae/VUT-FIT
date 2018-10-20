.PHONY: all clean test pack

all:
	rm -f antipirat
	ln antipirat.py antipirat
	chmod +x antipirat

clean:
	rm -f antipirat
	rm -f *.tgz
	rm -f *.xml
	rm -f *.txt
	rm -f *.torrent
	rm -f *.peerlist
	find . | grep -E "(__pycache__|\.pyc|\.pyo)" | xargs rm -rf

test: clean all
	bash do_tests.sh

pack: clean
	tar -cvzf xklima22.tgz *
