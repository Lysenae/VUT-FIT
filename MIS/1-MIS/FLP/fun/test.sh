#!/usr/bin/env bash

FILE='plg-2-nka'
if ! [ -f $FILE ]; then
   echo "Could not find $FILE"
fi

echo "Starting tests"
echo "#####"
echo

echo "Test 1 -- Invalid nonterminal"
echo "Input:"
cat 'test/test1'
echo
echo "Output:"
./$FILE -i 'test/test1'
echo "######"
echo

echo "Test 2 -- Invalid terminal"
echo "Input:"
cat 'test/test2'
echo
echo "Output:"
./$FILE -i 'test/test2'
echo "######"
echo

echo "Test 3 -- Invalid start symbol"
echo "Input:"
cat 'test/test3'
echo
echo "Output:"
./$FILE -i 'test/test3'
echo "######"
echo

echo "Test 4 -- Invalid symbol in the right side of the rule"
echo "Input:"
cat 'test/test4'
echo
echo "Output:"
./$FILE -i 'test/test4'
echo "######"
echo

echo "Test 5 -- Invalid format of the right side of the rule"
echo "Input:"
cat 'test/test5'
echo
echo "Output:"
./$FILE -i 'test/test5'
echo "######"
echo

echo "Test 6 -- Invalid nonterminal on the left side of the rule"
echo "Input:"
cat 'test/test6'
echo
echo "Output:"
./$FILE -i 'test/test6'
echo "######"
echo

echo "Test 7 -- Valid RLG (TIN ex. 3.8/28): Inner representation"
echo "Input:"
cat 'test/test7'
echo
echo "Output:"
./$FILE -i 'test/test7'
echo "######"
echo

echo "Test 8 -- Valid RLG (TIN ex. 3.8/28): Transformed"
echo "Input:"
cat 'test/test7'
echo
echo "Output:"
./$FILE -1 'test/test7'
echo "######"
echo

echo "Test 9 -- Valid RLG (TIN ex. 3.8/28): Equivalent NKA"
echo "Input:"
cat 'test/test7'
echo
echo "Output:"
./$FILE -2 'test/test7'
