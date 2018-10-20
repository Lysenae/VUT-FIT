
/*******************************************************************************
 * Súbor:   c202.c
 * Datum:   2014/10/12
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Dvojsmerne viazaný lineárny zoznam
 * Popis:   Implementácia dvojsmerne viazaného lineárneho zoznamu
 ******************************************************************************/

#include "c206.h"

int solved;
int errflg;

/*
** Vypí¹e upozornenie na to, ¾e do¹lo k chybe.
**/
void DLError()
{
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L)
{
/*
** Vykoná inicializáciu zoznamu L pred jeho prvým pou¾itím.
** @param L Ukazovateµ na zoznam
**/

  L->First = NULL;
  L->Last = NULL;
  L->Act = NULL;
}

/*
** Zru¹í v¹etky prvky zoznamu L a uvedie zoznam do stavu, v akom
** se nachádzal po inicializácii.
** @param L Ukazovateµ na zoznam
**/
void DLDisposeList (tDLList *L)
{
  while(L->First != NULL)
  {
    tDLElemPtr next = L->First->rptr;
    free(L->First);
    L->First = next;
  }

  L->First = NULL;
  L->Last = NULL;
  L->Act = NULL;
}

/*
** Vlo¾í nový prvok na zaèiatok zoznamu L. Vypí¹e chybové hlásenie ak nie je
** dostatok pamäti pre vytvorenie nového prvku.
** @param L Ukazovateµ na zoznam
** @param val Hodnota prvku
**/
void DLInsertFirst (tDLList *L, int val)
{
  struct tDLElem *elem = malloc(sizeof(struct tDLElem));
  if(elem == NULL)
  {
    DLError();
    return;
  }
  else
  {
    elem->data = val;
    elem->lptr = NULL;

    if(L->First == NULL)
    {
      elem->rptr = NULL;
      L->Last = elem;
    }
    else
    {
      elem->rptr = L->First;
      L->First->lptr = elem;
    }

    L->First = elem;
  }
}

/*
** Vlo¾í nový prvok na koniec zoznamu L Vypí¹e chybové hlásenie ak nie je
** dostatok pamäti pre vytvorenie nového prvku.
** @param L Ukazovateµ na zoznam
** @param val Hodnota prvku
**/
void DLInsertLast(tDLList *L, int val)
{
  struct tDLElem *elem = malloc(sizeof(struct tDLElem));
  if(elem == NULL)
  {
    DLError();
    return;
  }
  else
  {
    elem->data = val;
    elem->rptr = NULL;

    if(L->Last == NULL)
    {
      elem->lptr = NULL;
      L->First = elem;
    }
    else
    {
      elem->lptr = L->Last;
      L->Last->rptr = elem;
    }

    L->Last = elem;
  }
}

/*
** Nastaví aktivitu na prvý prvok seznamu L.
** @param L Ukazovateµ na zoznam
**/
void DLFirst (tDLList *L)
{
  L->Act = L->First;
}

/*
** Nastaví aktivitu na posledný prvok zoznamu L.
** @param L Ukazovateµ na zoznam
**/
void DLLast (tDLList *L)
{
  L->Act = L->Last;
}

/*
** Prostredníctvom parametru val vráti hodnotu prvého prvku zoznamu L.
** Ak je zoznam L prázdny, vypí¹e chybové hlásenie.
** @param L Ukazovateµ na zoznam
** @param val Ukazovateµ na integer, do ktorého sa ulo¾í hodnota prvého prvku
**/
void DLCopyFirst (tDLList *L, int *val)
{
  if(L->First == NULL)
  {
    DLError();
    return;
  }
  else
  {
    *val = L->First->data;
  }
}

/*
** Prostredníctvom parametru val vráti hodnotu posledného prvku zoznamu L.
** Ak je zoznam L prázdny, vypí¹e chybové hlásenie.
** @param L Ukazovateµ na zoznam
** @param val Ukazovateµ na integer, do kt. sa ulo¾í hodnota posledného prvku
**/
void DLCopyLast (tDLList *L, int *val)
{
  if(L->Last == NULL)
  {
    DLError();
    return;
  }
  else
  {
    *val = L->Last->data;
  }
}

/*
** Zru¹í prvý prvok zoznamu L. Ak bol prvý prvok aktívny, aktivita 
** sa stráca. Ak bol zoznam L prázdny, niè sa nedeje.
** @param L Ukazovateµ na zoznam
**/
void DLDeleteFirst (tDLList *L)
{
  if(L->First != NULL)
  {
    if(L->First == L->Act)
    {
      L->Act = NULL;
    }

    tDLElemPtr next = L->First->rptr;
    free(L->First);
    L->First = next;

    if(L->First != NULL)
    {
      L->First->lptr = NULL;
    }
    else
    {
      L->Last = NULL;
    }
  }
}

/*
** Zru¹í posledný prvok zoznamu L. Ak bol posledný prvok aktívny,
** aktivita zoznamu sa stráca. Ak bol zoznam L prázdny, niè sa nedeje.
** @param L Ukazovateµ na zoznam
**/
void DLDeleteLast (tDLList *L)
{
  if(L->First != NULL)
  {
    if(L->Last == L->Act)
    {
      L->Act = NULL;
    }

    tDLElemPtr prev = L->Last->lptr;
    free(L->Last);
    L->Last = prev;

    if(L->Last != NULL)
    {
      L->Last->rptr = NULL;
    }
    else
    {
      L->First = NULL;
    }
  }
  
}

void DLPostDelete (tDLList *L)
{
/*
** Zru¹í prvok zoznamu L za aktívnym prvkom. Ak je zoznam L neaktívny alebo ak
** je aktívny prvok posledným prvkom zoznamu, niè sa nedeje.
** @param L Ukazovateµ na zoznam
**/

  if(L->Act != NULL && L->Act != L->Last)
  {
    tDLElemPtr next_next = L->Act->rptr->rptr;
    free(L->Act->rptr);
    if(next_next != NULL)
    {
      next_next->lptr = L->Act;
      L->Act->rptr = next_next;
    }
    else // mazany prvok bol posledny prvok zoznamu
    {
      L->Act->rptr = NULL;
      L->Last = L->Act;
    }
  }
}

/*
** Zru¹í prvok pred aktívnym prvkom zoznamu L. Ak je zoznam L neaktívny alebo
** ak je aktívny prvok prvým prvkom zoznamu, niè sa nedeje.
** @param L Ukazovateµ na zoznam
**/
void DLPreDelete (tDLList *L)
{
  if(L->Act != NULL && L->Act != L->First)
  {
    tDLElemPtr prev_prev = L->Act->lptr->lptr;
    free(L->Act->lptr);
    if(prev_prev != NULL)
    {
      prev_prev->rptr = L->Act;
      L->Act->lptr = prev_prev;
    }
    else // mazany prvok bol prvy prvok zoznamu
    {
      L->Act->lptr = NULL;
      L->First = L->Act;
    }
  }
}

/*
** Vlo¾í prvek za aktívny prvok zoznamu L. Ak nebol zoznam L aktívny, niè se 
** nedeje. Vypí¹e chybové hlásenie ak nie je dostatok pamäti pre vytvorenie
** nového prvku.
** @param L Ukazovateµ na zoznam
** @param val Hodnota prvku
**/
void DLPostInsert (tDLList *L, int val)
{
  if(L->Act != NULL)
  {
    struct tDLElem *elem = malloc(sizeof(struct tDLElem));
    if(elem == NULL)
    {
      DLError();
      return;
    }
    else
    {
      tDLElemPtr next = L->Act->rptr;
      elem->data = val;
      elem->lptr = L->Act;
      elem->rptr = next;

      L->Act->rptr = elem;

      if(next != NULL)
      {
        next->lptr = elem;
      }

      if(L->Last == L->Act)
      {
        L->Last = elem;
      }
    }
  }
}

/*
** Vlo¾í prvek pred aktívny prvok zoznamu L. Ak nebol zoznam L aktívny, niè se 
** nedeje. Vypí¹e chybové hlásenie ak nie je dostatok pamäti pre vytvorenie
** nového prvku.
** @param L Ukazovateµ na zoznam
** @param val Hodnota prvku
**/
void DLPreInsert (tDLList *L, int val)
{
  if(L->Act != NULL)
  {
    struct tDLElem *elem = malloc(sizeof(struct tDLElem));
    if(elem == NULL)
    {
      DLError();
      return;
    }
    else
    {
      tDLElemPtr prev = L->Act->lptr;
      elem->data = val;
      elem->rptr = L->Act;
      elem->lptr = prev;

      L->Act->lptr = elem;

      if(prev != NULL)
      {
        prev->rptr = elem;
      }

      if(L->First == L->Act)
      {
        L->First = elem;
      }
    }
  }
}

/*
** Prostredníctvom parametru val vráti hodnotu aktívneho prvku zoznamu L.
** AK zoznam L nie je aktívny, vypí¹e chybové hlásenie.
** @param L Ukazovateµ na zoznam
** @param val Ukazovateµ na integer, do kt. sa ulo¾í hodnota posledného prvku
**/
void DLCopy (tDLList *L, int *val)
{
  if(L->Act == NULL)
  {
    DLError();
    return;
  }
  else
  {
    *val = L->Act->data;
  }
}

/*
** Prepí¹e hodnotu aktívneho prvku zoznamu L. Ak zoznam L nie je aktívny, 
** nerobí niè.
** @param L Ukazovateµ na zoznam
** @param val Nová hodnota prvku
**/
void DLActualize (tDLList *L, int val)
{
  if(L->Act != NULL)
  {
    L->Act->data = val;
  }
}

/*
** Posunie aktivitu na nasledujúci prvok zoznamu L. Ak nie je zoznam aktívny, 
** nerobí niè
** @param L Ukazovateµ na zoznam
**/
void DLSucc (tDLList *L)
{
  if(L->Act != NULL)
  {
    L->Act = L->Act->rptr;
  }
}

/*
** Posunie aktivitu na predchádzajúci prvok zoznamu L. Ak nie je zoznam aktívny, 
** nerobí niè
** @param L Ukazovateµ na zoznam
**/
void DLPred (tDLList *L)
{
  if(L->Act != NULL)
  {
    L->Act = L->Act->lptr;
  }
}

/*
** Ak zoznam L aktívny, vracia TRUE. V opaènom prípade vracia FALSE.
** @param L Ukazovateµ na zoznam
**/
int DLActive (tDLList *L)
{
  return (L->Act != NULL);
}

/* Koneic príkladu c206.c*/
