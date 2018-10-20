/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-10
 * Subor:   process.h
 * ************************************************************************** */

#ifndef PROCESS_H
#define PROCESS_H

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "str_vector.h"

#define MAX_FNAME 256

struct Process
{
  char *in_file;
  char *out_file;
  bool background;
  bool invalid;
  char *args;
  struct StrVector params;
};

void p_init(struct Process *p, const char *args);
void p_destroy(struct Process *p);
void p_set_args(struct Process *p, const char *args);
void p_set_background(struct Process *p);
void p_set_file(struct Process *p, char c);
void p_parse_args(struct Process *p);
bool p_is_valid(struct Process *p);
bool p_is_background(struct Process *p);
const char *p_in_file(struct Process *p);
const char *p_out_file(struct Process *p);
char **p_get_args(struct Process *p);

void p_print(struct Process *p);

#endif // PROCESS_H
