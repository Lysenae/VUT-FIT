
/*******************************************************************************
 * S�bor:   c202.c
 * Datum:   2014/10/12
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Dvojsmerne viazan� line�rny zoznam
 * Popis:   Implement�cia dvojsmerne viazan�ho line�rneho zoznamu
 ******************************************************************************/

#include "c206.h"

int solved;
int errflg;

/*
** Vyp�e upozornenie na to, �e do�lo k chybe.
**/
void DLError()
{
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L)
{
/*
** Vykon� inicializ�ciu zoznamu L pred jeho prv�m pou�it�m.
** @param L Ukazovate� na zoznam
**/

  L->First = NULL;
  L->Last = NULL;
  L->Act = NULL;
}

/*
** Zru�� v�etky prvky zoznamu L a uvedie zoznam do stavu, v akom
** se nach�dzal po inicializ�cii.
** @param L Ukazovate� na zoznam
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
** Vlo�� nov� prvok na za�iatok zoznamu L. Vyp�e chybov� hl�senie ak nie je
** dostatok pam�ti pre vytvorenie nov�ho prvku.
** @param L Ukazovate� na zoznam
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
** Vlo�� nov� prvok na koniec zoznamu L Vyp�e chybov� hl�senie ak nie je
** dostatok pam�ti pre vytvorenie nov�ho prvku.
** @param L Ukazovate� na zoznam
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
** Nastav� aktivitu na prv� prvok seznamu L.
** @param L Ukazovate� na zoznam
**/
void DLFirst (tDLList *L)
{
  L->Act = L->First;
}

/*
** Nastav� aktivitu na posledn� prvok zoznamu L.
** @param L Ukazovate� na zoznam
**/
void DLLast (tDLList *L)
{
  L->Act = L->Last;
}

/*
** Prostredn�ctvom parametru val vr�ti hodnotu prv�ho prvku zoznamu L.
** Ak je zoznam L pr�zdny, vyp�e chybov� hl�senie.
** @param L Ukazovate� na zoznam
** @param val Ukazovate� na integer, do ktor�ho sa ulo�� hodnota prv�ho prvku
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
** Prostredn�ctvom parametru val vr�ti hodnotu posledn�ho prvku zoznamu L.
** Ak je zoznam L pr�zdny, vyp�e chybov� hl�senie.
** @param L Ukazovate� na zoznam
** @param val Ukazovate� na integer, do kt. sa ulo�� hodnota posledn�ho prvku
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
** Zru�� prv� prvok zoznamu L. Ak bol prv� prvok akt�vny, aktivita 
** sa str�ca. Ak bol zoznam L pr�zdny, ni� sa nedeje.
** @param L Ukazovate� na zoznam
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
** Zru�� posledn� prvok zoznamu L. Ak bol posledn� prvok akt�vny,
** aktivita zoznamu sa str�ca. Ak bol zoznam L pr�zdny, ni� sa nedeje.
** @param L Ukazovate� na zoznam
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
** Zru�� prvok zoznamu L za akt�vnym prvkom. Ak je zoznam L neakt�vny alebo ak
** je akt�vny prvok posledn�m prvkom zoznamu, ni� sa nedeje.
** @param L Ukazovate� na zoznam
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
** Zru�� prvok pred akt�vnym prvkom zoznamu L. Ak je zoznam L neakt�vny alebo
** ak je akt�vny prvok prv�m prvkom zoznamu, ni� sa nedeje.
** @param L Ukazovate� na zoznam
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
** Vlo�� prvek za akt�vny prvok zoznamu L. Ak nebol zoznam L akt�vny, ni� se 
** nedeje. Vyp�e chybov� hl�senie ak nie je dostatok pam�ti pre vytvorenie
** nov�ho prvku.
** @param L Ukazovate� na zoznam
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
** Vlo�� prvek pred akt�vny prvok zoznamu L. Ak nebol zoznam L akt�vny, ni� se 
** nedeje. Vyp�e chybov� hl�senie ak nie je dostatok pam�ti pre vytvorenie
** nov�ho prvku.
** @param L Ukazovate� na zoznam
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
** Prostredn�ctvom parametru val vr�ti hodnotu akt�vneho prvku zoznamu L.
** AK zoznam L nie je akt�vny, vyp�e chybov� hl�senie.
** @param L Ukazovate� na zoznam
** @param val Ukazovate� na integer, do kt. sa ulo�� hodnota posledn�ho prvku
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
** Prep�e hodnotu akt�vneho prvku zoznamu L. Ak zoznam L nie je akt�vny, 
** nerob� ni�.
** @param L Ukazovate� na zoznam
** @param val Nov� hodnota prvku
**/
void DLActualize (tDLList *L, int val)
{
  if(L->Act != NULL)
  {
    L->Act->data = val;
  }
}

/*
** Posunie aktivitu na nasleduj�ci prvok zoznamu L. Ak nie je zoznam akt�vny, 
** nerob� ni�
** @param L Ukazovate� na zoznam
**/
void DLSucc (tDLList *L)
{
  if(L->Act != NULL)
  {
    L->Act = L->Act->rptr;
  }
}

/*
** Posunie aktivitu na predch�dzaj�ci prvok zoznamu L. Ak nie je zoznam akt�vny, 
** nerob� ni�
** @param L Ukazovate� na zoznam
**/
void DLPred (tDLList *L)
{
  if(L->Act != NULL)
  {
    L->Act = L->Act->lptr;
  }
}

/*
** Ak zoznam L akt�vny, vracia TRUE. V opa�nom pr�pade vracia FALSE.
** @param L Ukazovate� na zoznam
**/
int DLActive (tDLList *L)
{
  return (L->Act != NULL);
}

/* Koneic pr�kladu c206.c*/
