/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-10
 * Subor:   utils.h
 * ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>

char *str_bool(bool val);
char *str_dup(const char *s);
char *str_trim(const char *s);
int str_chrn(const char *s, char c);
int str_fstc(const char *, char c);
void str_set(char **dest, const char *src);
void str_deln(char *s, int i, int n);

#endif // UTILS_H
