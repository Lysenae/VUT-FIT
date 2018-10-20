/*******************************************************************************
 * S�bor:   c201.c
 * Datum:   2014/10/11
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Jednosmern� line�rny zoznam
 * Popis:   Implement�cia jednosmern�ho line�rneho zoznamu
 ******************************************************************************/

#include "c201.h"

int solved;
int errflg;

/*
** Vyp�e upozornenie na to, �e do�lo k chybe.
**/
void Error()
{
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;
}

/*
** Vykon� inicializ�ciu zoznamu L pred jeho prv�m pou�it�m.
** @param L Ukazovate� na zoznam, ktor� sa m� inicializova�
**/
void InitList (tList *L)
{
  L->Act = NULL;
  L->First = NULL;
}

/*
** Zru�� v�etky prvky zoznamu L a uvedie zoznam L do stavu, v akom se nach�dzal
** po inicializ�cii. Pam� pou��van� prvkami zoznamu L bude sa uvoln�.
** @param L Ukazovate� na zoznam, ktor�ho prvky sa maj� zru�i�
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
** Vlo�� prvok s hodnotou val na za�iatok zoznamu L. V pr�pade nedostatku
** pam�ti vyp�e chybov� hl�senie.
** @param L Ukazovate� na zoznam, do ktor�ho sa bude vklada� prvok
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
** Nastav� aktivitu zoznamu L na jeho prv� prvok.
** @param L Ukazovate� na zoznam, v ktorom sa aktivuje prvok
**/
void First (tList *L)
{
  L->Act = L->First;
}

/*
** Prostredn�ctvom parametru val vr�ti hodnotu prvn�ho prvku zoznamu L alebo
** vyp�e chybu, ak je zoznam pr�zdny.
** @param L Ukazovate� na zoznam, z ktor�ho sa ��ta hodnota
** @param val Ukazovate� na integer, do ktor�ho sa ulo�� hodnota prvku
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
** Zru�� prv� prvok zoznamu L a uvoln� n�m pou��van� pam�.
** @param L Ukazovate� na zoznam, v ktorom sa m� zru�i� prvok
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
** Zru�� prvok zoznamu L za akt�vnym prvkom a uvoln� n�m pou��van� pam�.
** @param L Ukozovate� na zoznam, v ktorom sa m� zru�i� prvok
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
** Vlo�� prvok s hodnotou val za akt�vny prvok zoznamu L. Ak nie je dostatok
** pam�ti, vyp�e chybov� hl�senie.
** @param L Ukazovate� na zoznam, do ktor�ho sa bude vklada� prvok
** @param val Hodnota vkladan�ho prvku
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
** Prostredn�ctvom parametru val vr�ti hodnotu akt�vneho prvku zoznamu L.
** Ak zoznam nie je aktivn�, vyp�e chybu.
** @param L Ukazovate� na zoznam, z ktor�ho sa z�skava hodnota prvku
** @pram val Ukazovate� na integer, do ktor�ho sa ulo�� hodnota akt�vneho prvku
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
** Prep�e hodnotu akt�vneho prvku zoznamu L hodnotou val.
** @param L Ukazovate� na zoznam, v ktorom sa m� zmeni� hodnota prvku
** @param val Nov� hodnota prvku
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
** Posunie aktivitu na nasleduj�ci prvok zoznamu L, ak je zoznam akt�vny.
** @param L Ukazovate� na zoznam, v ktorom sa m� posun�� aktivita
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
** Ak je zoznam L akt�vny, vracia TRUE. V opa�nom pr�pade vracia FALSE.
** @param L Ukazovate� na zoznam, u ktor�ho sa zis�uje �i je akt�vny
**/
int Active (tList *L)
{
  return (L->Act != NULL);
}

/* Koniec pr�kladu c201.c */
