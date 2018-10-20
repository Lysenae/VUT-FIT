#!/bin/bash

# File:        do_test.sh
# Author:      Daniel Klimaj; xklima22@stud.fit.vutbr.cz
# Description: Tests for 2nd IPK Project

# How long to sleep?
TIME=1

# Exceutables' names
CLIENT="ipkhttpclient"
SERVER="ipkhttpserver"

# Create test directories
mkdir -p tests/{test1,test2,test3,test4,test5}/www

# Copy test text files into newly created www directories
cp test_text.txt tests/test1/www/test.txt
cp test_text.txt tests/test2/www/test.txt
cp test_text.txt tests/test3/www/test.txt

# Copy executables into new directories
cp ipkhttpclient tests/test1/ipkhttpclient
cp ipkhttpclient tests/test2/ipkhttpclient
cp ipkhttpclient tests/test3/ipkhttpclient
cp ipkhttpclient tests/test4/ipkhttpclient
cp ipkhttpclient tests/test5/ipkhttpclient

cp ipkhttpserver tests/test1/ipkhttpserver
cp ipkhttpserver tests/test2/ipkhttpserver
cp ipkhttpserver tests/test3/ipkhttpserver
cp ipkhttpserver tests/test4/ipkhttpserver
cp ipkhttpserver tests/test5/ipkhttpserver

# Tests
echo
echo "#########################"
echo "<--- Beginning tests --->"

# Test #1: IpkClient requests IpkServer for existing file, chunk size unspecified
echo "#########################"
echo "<--- Start - Test 1 --->"
exec ./tests/test1/$SERVER -p 10001 &
pid=$!
sleep $TIME
(exec ./tests/test1/$CLIENT "http://127.0.0.1:10001/test.txt")
sleep $TIME
kill -INT "$pid"
echo "<--- End - Test 1 --->"
echo

# Test #2: IpkClient requests IpkServer for existing file, chunk size specified
echo "#########################"
echo "<--- Start - Test 2 --->"
exec ./tests/test2/$SERVER -p 10002 -c 80 &
pid=$!
sleep $TIME
(exec ./tests/test2/$CLIENT "http://127.0.0.1:10002/test.txt")
sleep $TIME
kill -INT "$pid"
echo "<--- End - Test 2 --->"
echo

# Test #3: IpkClient requests IpkServer for existing file, chunked transfer
echo "#########################"
echo "<--- Start - Test 3 --->"
exec ./tests/test3/$SERVER -p 10003 -c 80 -t 0 &
pid=$!
sleep $TIME
(exec ./tests/test3/$CLIENT "http://127.0.0.1:10003/test.txt")
sleep $TIME
kill -INT "$pid"
echo "<--- End - Test 3 --->"
echo

# Test #4: IpkClient requests IpkServer for nonexisting file
echo "#########################"
echo "<--- Start - Test 4 --->"
exec ./tests/test4/$SERVER -p 10004 &
pid=$!
sleep $TIME
(exec ./tests/test4/$CLIENT "http://127.0.0.1:10004/test.txt")
sleep $TIME
kill -INT "$pid"
echo "<--- End - Test 4 --->"
echo

# Test #5: IpkClient sends request to www.example.com
echo "#########################"
echo "<--- Start - Test 5 --->"
(exec ./tests/test5/$CLIENT "http://www.example.com")
echo "<--- End - Test 5 --->"

echo "#########################"
echo "<--- Tests Completed --->"
echo "#########################"

# do_test.sh
