/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-10
 * Subor:   str_vector.h
 * ************************************************************************** */

#ifndef STRVECTOR_H
#define STRVECTOR_H

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Dynamicke pole retazcov
struct StrVector
{
  char **vals;
  int length;
};

void v_init(struct StrVector *v);
void v_destroy(struct StrVector *v);
void v_append(struct StrVector *v, const char *s);
int v_size(struct StrVector *v);
char *v_at(struct StrVector *v, int idx);
char **v_to_a(struct StrVector *v);
void v_print(struct StrVector *v);

#endif // STRVECTOR_H
