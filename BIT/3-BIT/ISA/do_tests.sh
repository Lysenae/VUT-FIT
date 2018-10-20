#!/bin/bash

# Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Popis: Testy k projektu ISA - Analyza BitTorrent

echo '#####################################################'
echo '# Test #1 - parameter -r                            #'
echo '#####################################################'
./antipirat -r 'https://kat.cr/movies/?rss=1'
echo '#####################################################'
echo '# Test #1 - koniec                                  #'
echo '#####################################################'
echo
sleep 1

echo '#####################################################'
echo '# Test #2 - parameter -i                            #'
echo '#####################################################'
./antipirat -i 'test/movies_announce.xml'
echo '#####################################################'
echo '# Test #2 - koniec                                  #'
echo '#####################################################'
echo
sleep 1

echo '#####################################################'
echo '# Test #3 - parameter -t                            #'
echo '#####################################################'
./antipirat -t 'test/el.corredor.del.laberinto.las.pruebas.2015.spanish.espa%C3%B1ol.hdrip.xvid.newpct.torrent'
echo '#####################################################'
echo '# Test #3 - koniec                                  #'
echo '#####################################################'
echo
sleep 1

echo '#####################################################'
echo '# Test #4 - parameter -t a parameter -a             #'
echo '#####################################################'
./antipirat -t 'test/steel.dawn.1987.remastered.bdrip.x264.veto.rarbg.torrent' \
-a 'http://tracker.trackerfix.com/announce'
echo '#####################################################'
echo '# Test #4 - koniec                                  #'
echo '#####################################################'

# do_test.sh
