// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

typedef std::vector<std::string> StrVect;
typedef std::vector<uchar>       UchrVect;

const uint BITS         = 8;
const uint B_UCHAR_MIN  = 0;
const uint B_UCHAR_MAX  = 255;
const int  OP_FAIL      = -1;
const int  OP_SUCC      = 0;
const uint S_UCHAR      = 1;
const uint S_USHORT     = 2;

std::string str_bytes16(uint16_t t);
std::string str_bytes8(uchar t);
uchar str_to_uch(std::string s);
uchar literal_to_uchr(std::string s);
StrVect split_str(std::string s, char delimiter);

#endif // TYPES_H
