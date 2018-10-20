/*******************************************************************************
 * S�bor:   c203.c
 * Datum:   2014/10/11
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Fronta znakov v poli
 * Popis:   Implement�cia fronty znakov v poli
 ******************************************************************************/

#include "c203.h"

/*
** Vyp�e upozornenie na to, �e do�lo k chybe.
** @param error_code Chybov� k�d
*/
void queueError (int error_code)
{
  static const char* QERR_STRINGS[MAX_QERR+1] = {
    "Unknown error",
    "Queue error: UP",
    "Queue error: FRONT",
    "Queue error: REMOVE",
    "Queue error: GET",
    "Queue error: INIT"
  };

  if ( error_code <= 0 || error_code > MAX_QERR )
    error_code = 0;

  printf ( "%s\n", QERR_STRINGS[error_code] );
  err_flag = 1;
}

/*
** Inicializujte frontu.
** @param q Ukazovate� na frontu
*/
void queueInit (tQueue* q)
{
  // Test, ci nie je predavany NULL
  if(q == NULL)
  {
    queueError(QERR_INIT);
    return;
  }

  // Naplnenie fronty
  for(int i=0; i<QUEUE_SIZE; i++)
    q->arr[i] = '*';

  // Nastavenie indexov
  q->f_index = 0;
  q->b_index = 0;
}

/*
** Vracia index nasleduj�ceho prvku v poli.
** @param index Index prvku po�a, pre ktor� sa zis�uje index nasleduj�ceho prvku
*/
int nextIndex (int index)
{
  return (index+1) % QUEUE_SIZE;
}

/*
** Vracia nenulov� hodnotu, ak je frona pr�zdn�, inak vracia hodnotu 0. 
** @param q Ukazovate� na frontu
*/
int queueEmpty (const tQueue* q)
{
  return (q->f_index == q->b_index);
}

/*
** Vracia nenulov� hodnotu, ak je fronta pln�, inak vracia hodnotu 0. 
** @param q Ukazovate� na frontu
*/
int queueFull (const tQueue* q)
{
  return (nextIndex(q->b_index) == q->f_index);
}

/*
** Prostredn�ctvom parametru c vr�ti znak zo za�iatku fronty q.
** Ak je fronta pr�zdna, tak vyp�e chybov� hl�senie.
** @param q Ukazovate� na frontu
** @param c Ukazovate� na char, do ktor�ho sa ulo�� znak zo za�iatku fronty
*/
void queueFront (const tQueue* q, char* c)
{
  if(queueEmpty(q))
  {
    queueError(QERR_FRONT);
    return;
  }
  else
  {
    *c = q->arr[q->f_index];
  }
}

/*
** Odstr�ni znak zo za�iatku fronty q. Ak je fronta pr�zdna, tak vyp�e
** chybov� hl�senie.
** @param q Ukazovate� na frontu
*/
void queueRemove (tQueue* q)
{
  if(queueEmpty(q))
  {
    queueError(QERR_REMOVE);
    return;
  }
  else
  {
    q->f_index = nextIndex(q->f_index);
  }
}

/*
** Odstr�ni znak zo za�iatku fronty a vr�ti ho prostredn�ctvom parametru c.
** Ak je fronta pr�zdna, tak vyp�e chybov� hl�senie.
** @param q Ukazovate� na frontu
** @param c Ukazovate� na char, do ktor�ho sa ulo�� znak zo za�iatku fronty
*/
void queueGet (tQueue* q, char* c)
{
  if(queueEmpty(q))
  {
    queueError(QERR_GET);
    return;
  }
  else
  {
    queueFront(q, c);
    queueRemove(q);
  }
}

/*
** Vlo�� znak c do fronty. Ak je fronta pln�, tak vyp�e chybov� hl�senie.
** @param q Ukazovate� na frontu
** @param c Znak, ktor� sa vlo�� na koniec fronty
*/
void queueUp (tQueue* q, char c)
{
  if(queueFull(q))
  {
    queueError(QERR_UP);
    return;
  }
  else
  {
    q->arr[q->b_index] = c;
    q->b_index = nextIndex(q->b_index);
  }
}

/* Koniec pr�kladu c203.c */
