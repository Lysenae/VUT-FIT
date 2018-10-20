/*******************************************************************************
 * Súbor:   c201.c
 * Datum:   2014/10/11
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Jednosmerný lineárny zoznam
 * Popis:   Implementácia jednosmerného lineárneho zoznamu
 ******************************************************************************/

#include "c201.h"

int solved;
int errflg;

/*
** Vypí¹e upozornenie na to, ¾e do¹lo k chybe.
**/
void Error()
{
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;
}

/*
** Vykoná inicializáciu zoznamu L pred jeho prvým pou¾itím.
** @param L Ukazovateµ na zoznam, ktorý sa má inicializova»
**/
void InitList (tList *L)
{
  L->Act = NULL;
  L->First = NULL;
}

/*
** Zru¹í v¹etky prvky zoznamu L a uvedie zoznam L do stavu, v akom se nachádzal
** po inicializácii. Pamä» pou¾ívaná prvkami zoznamu L bude sa uvolní.
** @param L Ukazovateµ na zoznam, ktorého prvky sa majú zru¹i»
**/
void DisposeList (tList *L)
{
  while(L->First != NULL)
  {
    tElemPtr next = L->First->ptr;
    free(L->First);
    L->First = next;
  }

  L->First = NULL;
  L->Act = NULL;
}

/*
** Vlo¾í prvok s hodnotou val na zaèiatok zoznamu L. V prípade nedostatku
** pamäti vypí¹e chybové hlásenie.
** @param L Ukazovateµ na zoznam, do ktorého sa bude vklada» prvok
** @param val Hodnota prvku
**/
void InsertFirst (tList *L, int val)
{
  // Alokacia a test uspesnosti alokacie pamati
  struct tElem *elem = malloc(sizeof(struct tElem));
  if(elem == NULL)
  {
    Error();
    return;
  }

  elem->data = val;

  if(L->First == NULL)
  {
    elem->ptr = NULL;
    L->First = elem;
  }
  else
  {
    elem->ptr = L->First;
    L->First = elem;
  }
}

/*
** Nastaví aktivitu zoznamu L na jeho prvý prvok.
** @param L Ukazovateµ na zoznam, v ktorom sa aktivuje prvok
**/
void First (tList *L)
{
  L->Act = L->First;
}

/*
** Prostredníctvom parametru val vráti hodnotu prvného prvku zoznamu L alebo
** vypí¹e chybu, ak je zoznam prázdny.
** @param L Ukazovateµ na zoznam, z ktorého sa èíta hodnota
** @param val Ukazovateµ na integer, do ktorého sa ulo¾í hodnota prvku
**/
void CopyFirst (tList *L, int *val)
{
  // Test, ci je zoznam inicializovany
  if(L->First == NULL)
  {
    Error();
    return;
  }
  else
  {
    *val = L->First->data;
  }
}

/*
** Zru¹í prvý prvok zoznamu L a uvolní ním pou¾ívanú pamä».
** @param L Ukazovateµ na zoznam, v ktorom sa má zru¹i» prvok
**/
void DeleteFirst (tList *L)
{
  // Test, ci je zoznam inicializovany
  if(L->First != NULL)
  {
    if(L->First == L->Act)
      L->Act = NULL;

    tElemPtr next = L->First->ptr;
    free(L->First);
    L->First = next;
  }
}

/* 
** Zru¹í prvok zoznamu L za aktívnym prvkom a uvolní ním pou¾ívanú pamä».
** @param L Ukozovateµ na zoznam, v ktorom sa má zru¹i» prvok
**/
void PostDelete (tList *L)
{
  //Test, ci je zoznam aktivny a nie je aktivny posledny prvok zoznamu
  if(L->Act != NULL && L->Act->ptr != NULL)
  {
    tElemPtr next = L->Act->ptr->ptr;
    free(L->Act->ptr);
    L->Act->ptr = next;
  }
}

/*
** Vlo¾í prvok s hodnotou val za aktívny prvok zoznamu L. Ak nie je dostatok
** pamäti, vypí¹e chybové hlásenie.
** @param L Ukazovateµ na zoznam, do ktorého sa bude vklada» prvok
** @param val Hodnota vkladaného prvku
**/
void PostInsert (tList *L, int val)
{
  // Test, ci je zoznam aktivny
  if(L->Act != NULL)
  {
    struct tElem *elem = malloc(sizeof(struct tElem));
    if(elem == NULL) // Chyba pri alokacii pamati
    {
      Error();
      return;
    }
    else
    {
      tElemPtr next = L->Act->ptr;
      elem->data = val;
      elem->ptr = next;
      L->Act->ptr = elem;
    }
  }
}

/*
** Prostredníctvom parametru val vráti hodnotu aktívneho prvku zoznamu L.
** Ak zoznam nie je aktivní, vypí¹e chybu.
** @param L Ukazovateµ na zoznam, z ktorého sa získava hodnota prvku
** @pram val Ukazovateµ na integer, do ktorého sa ulo¾í hodnota aktívneho prvku
**/
void Copy (tList *L, int *val)
{
  // Test, ci je zoznam aktivny
  if(L->Act == NULL)
  {
    Error();
    return;
  }
  else
  {
    *val = L->Act->data;
  }
}

/*
** Prepí¹e hodnotu aktívneho prvku zoznamu L hodnotou val.
** @param L Ukazovateµ na zoznam, v ktorom sa má zmeni» hodnota prvku
** @param val Nová hodnota prvku
**/
void Actualize (tList *L, int val)
{
  // Test, ci je zoznam aktivny
  if(L->Act != NULL)
  {
    L->Act->data = val;
  }
}

/*
** Posunie aktivitu na nasledujúci prvok zoznamu L, ak je zoznam aktívny.
** @param L Ukazovateµ na zoznam, v ktorom sa má posunú» aktivita
**/
void Succ (tList *L)
{
  // Test, ci je zoznam aktivny
  if(L->Act != NULL)
  {
    L->Act = L->Act->ptr;
  }
}

/*
** Ak je zoznam L aktívny, vracia TRUE. V opaènom prípade vracia FALSE.
** @param L Ukazovateµ na zoznam, u ktorého sa zis»uje èi je aktívny
**/
int Active (tList *L)
{
  return (L->Act != NULL);
}

/* Koniec príkladu c201.c */
