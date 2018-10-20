/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-10
 * Subor:   str_vector.c
 * ************************************************************************** */

#include "str_vector.h"

/**
 * Inicializuje vektor.
 * @param v StrVector
**/
void v_init(struct StrVector *v)
{
  v->vals   = NULL;
  v->length = 0;
}

/**
 * Zrusi vektor.
 * @param v StrVector
**/
void v_destroy(struct StrVector *v)
{
  for(int i=0; i<v_size(v); ++i)
  {
    free(v->vals[i]);
    v->vals[i] = NULL;
  }
  free(v->vals);
  v_init(v);
}

/**
 * Prida novy prvok na koniec vektoru.
 * @param v StrVector
 * @param s Vkladany retazec
**/
void v_append(struct StrVector *v, const char* s)
{
  size_t len = strlen(s);
  v->vals = (char**)realloc(v->vals, sizeof(char *)*(v->length+1));
  v->vals[v->length] = (char *)malloc(sizeof(char)*(len+1));
  strcpy(v->vals[v->length], s);
  v->vals[v->length][len] = '\0';
  v->length += 1;
}

/**
 * Zisti pocet prvkov vektoru.
 * @param v StrVector
 * @return pocet prvkov vektoru
**/
int v_size(struct StrVector *v)
{
  return v->length;
}

/**
 * Vrati prvok na danej pozicii.
 * @param v StrVector
 * @param idx Index pozadovaneho prvku
 * @return prvok na danej pozicii alebo NULL
**/
char *v_at(struct StrVector *v, int idx)
{
  if(idx >= 0 && idx < v_size(v))
  {
    size_t len = strlen(v->vals[idx]);
    char *s = (char *)malloc(sizeof(char)*(len+1));
    strcpy(s, v->vals[idx]);
    s[len] = '\0';
    return s;
  }
  return NULL;
}

/**
 * Vrati pole retazcov ulozenych vo vektore.
 * @param v StrVector
 * @return pole albo NULL
**/
char **v_to_a(struct StrVector *v)
{
  if(v_size(v) > 0)
  {
    char **a;
    int len;
    a = (char **)malloc(sizeof(char *)*(v_size(v)+1));
    for(int i=0; i<v_size(v); ++i)
    {
      len = strlen(v_at(v, i));
      a[i] = (char *)malloc(sizeof(char)*(len+1));
      strcpy(a[i], v_at(v, i));
      a[i][len] = '\0';
    }
    a[v_size(v)] = NULL;
    return a;
  }
  return NULL;
}

/**
 * Vypise obsah vektoru.
 * @param v StrVector
**/
void v_print(struct StrVector *v)
{
  for(int i=0; i<v_size(v); ++i)
    printf("%s\n", v_at(v, i));
}
