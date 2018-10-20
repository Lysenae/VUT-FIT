/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-10
 * Subor:   utils.c
 * ************************************************************************** */

#include "utils.h"

/**
 * Vypise bool.
 * @param val bool
 * @return retazec bool
**/
char *str_bool(bool val)
{
  return val ? "true" : "false";
}

/**
 * Vytvori kopiu retazca.
 * @param s retazec
 * @return kopiu retazca
**/
char *str_dup(const char *s)
{
  size_t len = strlen(s);
  char *ns = (char *)calloc(sizeof(char), len+1);
  strcpy(ns, s);
  return ns;
}

/**
 * Odstrani biele znaky na zaciatku a konci retazca. Prebrane z
 * http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-
 * whitespace-in-a-standard-way.
 * @param is retazec
 * @return ocisteny retazec
**/
char *str_trim(const char *is)
{
  char *s = str_dup(is);
  char *p = s;
  int l = strlen(p);
  while(isspace(p[l - 1])) p[--l] = 0;
  while(* p && isspace(* p)) ++p, --l;
  memmove(s, p, l + 1);
  return s;
}

/**
 * Zisti pocet vyskytov znaku v retazci.
 * @param s retazec
 * @return pocet vyskytov znaku v retazci
**/
int str_chrn(const char *s, char c)
{
  int count = 0;
  for(unsigned int i=0; i<strlen(s); ++i)
  {
    if(s[i] == c)
      ++count;
  }
  return count;
}

/**
 * Najde index prveho vyskytu znaku.
 * @param s retazec
 * @param c hladany znak
 * @return index vyskytu znaku
**/
int str_fstc(const char *s, char c)
{
  int idx = -1;
  unsigned int i   = 0;
  while(i<strlen(s))
  {
    if(s[i] == c)
    {
      idx = (int)i;
      break;
    }
    ++i;
  }
  return idx;
}

/**
 * Nastavi retazec na danu hodnotu.
 * @param dest cielovy retazec
 * @param src zdrojovy retazec
**/
void str_set(char **dest, const char *src)
{
  size_t len = strlen(src);
  *dest = (char *)calloc(sizeof(char), len+1);
  strcpy(*dest, src);
}

/**
 * Odstrani zadany pocet znakov pocnuc zadanym indexom.
 * @param s retazec
 * @param i pociatocny index
 * @param pocet odstranenych znakov
**/
void str_deln(char *s, int i, int n)
{
  if(i >= 0 && i < (int)strlen(s))
  {
    for(int j=0; j<n; ++j)
      memmove(&s[i], &s[i + 1], strlen(s) - i);
  }
}
