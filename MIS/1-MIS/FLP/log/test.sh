#!/usr/bin/env bash

FILE='flp17-log'
if ! [ -f $FILE ]; then
   echo "Nepodarilo sa najst $FILE"
   exit
fi

echo "Starting tests"
echo "#####"
echo

echo "Test 1 - Ukazkovy TS zo zadania"
echo "Vstup:"
cat 'test/test1.in'
echo
echo "Vystup:"
./$FILE < 'test/test1.in'
echo "######"
echo

echo "Test 2 - Nedeterminisicky vyber"
echo "Vstup:"
cat 'test/test2.in'
echo
echo "Vystup:"
./$FILE < 'test/test2.in'
echo "######"
echo

echo "Test 3 - Neplatna konfiguracia"
echo "Vstup:"
cat 'test/test3.in'
echo
echo "Vystup:"
./$FILE < 'test/test3.in'
echo "######"
echo

echo "Test 4 - Nedeterministicky vyber, komplikovanejsi TS"
echo "Vstup:"
cat 'test/test4.in'
echo
echo "Vystup:"
./$FILE < 'test/test4.in'
echo "######"
echo

echo "Test 5 - Posun za lavy okraj pasky"
echo "Vstup:"
cat 'test/test5.in'
echo
echo "Vystup:"
./$FILE < 'test/test5.in'
echo "######"
echo

echo "Test 6 - Vyber medzi posunom za lavy okraj pasky alebo spravnym ukoncenim"
echo "Vstup:"
cat 'test/test6.in'
echo
echo "Vystup:"
./$FILE < 'test/test6.in'
echo "######"
echo

echo "Test 7 - Nedeterministicky vyber medzi nekonecnym cyklom a spravnym ukoncenim"
echo "Vstup:"
cat 'test/test7.in'
echo
echo "Vystup:"
./$FILE < 'test/test7.in'
echo "######"
echo

echo "Test 8 - Nedosiahnutelny koncovy stav"
echo "Vstup:"
cat 'test/test8.in'
echo
echo "Vystup:"
./$FILE < 'test/test8.in'
echo "######"
echo

echo "Test 9 - Nekonecny cyklus, ukoncenie na nedostatok pamati"
echo "Vstup:"
cat 'test/test9.in'
echo
echo "Vystup:"
./$FILE < 'test/test9.in'
